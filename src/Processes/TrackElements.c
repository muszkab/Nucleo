/*
 * TrackElements.c
 *
 *  Created on: 2017. jan. 17.
 *      Author: Muszka
 */

#include "TrackElements.h"
#include "LineType.h"
#include "WallType.h"
#include "PositionControl.h"
#include "MotorControl.h"
#include "stdlib.h"

/* V�ltoz�k */
//�llapotv�ltoz�, milyen akad�lyt teljes�t �pp
static State_TrackElement TrackElement;

/*  Static f�ggv�nyek */
static void Do_Normal_TrackElement();
static void Do_Roundabout_TrackElement();
static void Look_and_Set_TrackElement(State_LineType StateLineType, State_Wall StateWall);
static void Go_Normal(const float Velocity, const Go_Type GoType);
static void Go_Xcm(const float TotalDistance, const float Velocity, const Go_Type GoType);

void Do_SkillTrack()
{
	switch(TrackElement){
	case(Normal_E):
			Do_Normal_TrackElement();
			break;

	case(Start_E):
			break;
	case(Finish_E):
			break;
	case(Dron):
			break;
	case(TrailerOn):
			break;
	case(TrailerOff):
			break;
	case(Zebra):
			break;
	case(Teeter):
			break;
	case(Barrel):
			break;
	case(Roundabout):
			Do_Roundabout_TrackElement();
			break;
	}

}

/* Csak megy a megadott sebess�ggel, �s  k�veti a vonalat, ha azt k�rt�k.
 * Do_PositionControl_AT()-val �s Do_MotorControl()-val implement�lva */
static void Go_Normal(const float Velocity, const Go_Type GoType)
{
	MotorControlSetVelocityRef(Velocity);
	Do_MotorControl();
	//ha be van kapcsolva a vonal, k�vesse a vonalat is, egy�bk�nt meg ne
	if(GoType == Go_WithLine)
	{
		Do_PositionControl_AT();
	}
}

/* Annyi centim�tert megy el�re(pozit�v) vagy h�tra(negat�v), amennyit param�terben megkap.
 * Sebess�get is lehet �ll�tani param�terben.
 * TotalDistance: [cm] pozit�v �s negat�v(el�re-h�tra)
 * Velocity: [m/s] csak pozit�v
 */
static void Go_Xcm(const float TotalDistance, const float Velocity, const Go_Type GoType)
{
	//ha a sebess�g negat�v, ne csin�ljon semmit
	if(Velocity < 0)
		return;

	//abszol�t pont lek�r�se, ehhez fogunk viszony�tani
	float StartPos = Encoder_GetDistance_cm();
	//megtett t�vols�g jelenleg
	float CurrentDistance = 0;
	//mekkora sebess�ggel menjen, ha TotalDistance negat�v, ez is negat�v kell legyen
	float t_velocity;

	//ha h�trafel� kell menni, negat�v sebess�get kell a szab�lyz�nak be�ll�tani
	if(TotalDistance < 0)
	{
		t_velocity = -Velocity;
	}
	else
	{
		//pozit�v sebess�ggel ind�t�s
		t_velocity = Velocity;
	}

	//am�g nem tette meg a k�v�nt t�vols�got
	while(abs(CurrentDistance) < abs(TotalDistance))
	{
		//Vonalk�vet�s
		Go_Normal(t_velocity, GoType);
		//megtett �t kisz�m�t�sa
		CurrentDistance = Encoder_GetDistance_cm() - StartPos;
	}

	//el�rt�k a k�v�nt t�vols�got, meg kell �llni
	while((int)Encoder_GetVelocity() != 0)
	{
		Go_Normal(0, GoType);
	}
}

//Alap vonalk�vet�shez tartoz� algoritmusok
static void Do_Normal_TrackElement()
{
	//akad�lykeres�s
	Look_and_Set_TrackElement(Get_StateLineType(), Get_StateWall());

	//Vonalt�pus meghat�roz�s
	Do_LineType();

	//Falt�pus meghat�roz�s
	Do_WallType();

	//Vonalk�vet�s: �llapotteres szab�lyz�
	Do_PositionControl_AT();

	//Sebess�g: fix �rt�k vonalt�pus alapj�n, Q1
	//Do_SpeedControl_FixSpeed();
}

//K�rforgalom algoritmus
static void Do_Roundabout_TrackElement()
{
	Go_Xcm(20, 1, Go_WithLine);
}

/* figyeli az akad�lyt jelz� �llapotv�ltoz�kat(StateLineType �s StateWall),
 * �s azoknak megfelel�en �ll�tja a TrackElement-et, ami mutatja milyen akad�ly van �pp
 */
static void Look_and_Set_TrackElement(State_LineType StateLineType, State_Wall StateWall)
{
	//vonallal jelzett akad�lyok
	switch(StateLineType){
		case(Egyvonal_szagg):
				TrackElement = Roundabout;
		break;

		case(Ketvonal_szagg):
				TrackElement = Barrel;
		break;

		case(Haromvonal_folyt):
				TrackElement = Dron;
		break;

		case(Haromvonal_szagg):
				TrackElement = Zebra;
		break;

		case(Keresztvonal):
				TrackElement = Finish_E;
		break;

		default:
		break;
	}
	//fallal jelzett akad�lyok
	switch(StateWall){
		case(Simafal_jobb):
				TrackElement = Teeter;
		break;

		case(Simafal_bal):
				TrackElement = Teeter;
		break;

		case(Bordasfal_jobb):
				TrackElement = TrailerOff;
		break;

		case(Bordasfal_bal):
				TrackElement = TrailerOff;
		break;

		case(Kapu):
		break;

		default:
		break;
	}

}

//minden �llapotv�ltoz�t alap�llapotba rak
void Set_TrackElement_Normal()
{
	//p�lyaelem
	TrackElement = Normal_E;
	//vonalt�pus
	Set_StateLineType(Egyvonal_folyt);
	//falt�pus
	Set_StateWall(Nincsfal);
}

/* Get f�ggv�nyek */
State_TrackElement Get_TrackElement()
{
	return TrackElement;
}
