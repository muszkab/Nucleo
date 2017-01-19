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
#include "Message.h"
#include "stdlib.h"

/* Akad�ly param�terek */
#define LOW_SPEED			0.6f	//alacsony sebess�g
#define DRON_DISTANCE		30
#define NO_DRON_DISTANCE	100

//Poz�ci� szab�lyoz�s: PD vagy �llapotteres TODO: PD behangol�s!
//#define Do_PositionControl()	Do_PositionControl_PD()
#define Do_PositionControl()	Do_PositionControl_AT()

/* V�ltoz�k */
//�llapotv�ltoz�, milyen akad�lyt teljes�t �pp
static State_TrackElement TrackElement = Finish_E;

/*  Static f�ggv�nyek */
/* Akad�lyok */
static void Do_Normal_TrackElement(const float VelocityRef);
static void Do_Roundabout_TrackElement();
static void Do_Dron_TrackElement(const float VelocityRef);
/* Seg�df�ggv�nyek */
static void Look_and_Set_TrackElement(State_LineType StateLineType, State_Wall StateWall);
static void Go_Normal(const float Velocity, const Go_Type GoType);
static void Go_Xcm(const float TotalDistance, const float Velocity, const Go_Type GoType);

void Do_SkillTrack()
{
	switch(TrackElement){
	case(Normal_E):
			Do_Normal_TrackElement(LOW_SPEED);
			//Go_Xcm(130, 1, Go_WithLine);
			break;
	case(Start_E):
		Do_Normal_TrackElement(0);
			break;
	case(Finish_E):
			Do_Normal_TrackElement(0);
			break;
	case(Dron):
			Do_Dron_TrackElement(LOW_SPEED);
			break;
	case(TrailerOn):
		Do_Normal_TrackElement(0);
			break;
	case(TrailerOff):
		Do_Normal_TrackElement(0);
			break;
	case(Zebra):
		Do_Normal_TrackElement(0);
			break;
	case(Teeter):
		Do_Normal_TrackElement(0);
			break;
	case(Barrel):
		Do_Normal_TrackElement(0);
			break;
	case(Roundabout):
		Do_Normal_TrackElement(0);
			break;
	}

}

/* Csak megy a megadott sebess�ggel, �s  k�veti a vonalat, ha azt k�rt�k.
 * Do_PositionControl()-val �s Do_MotorControl()-val implement�lva */
static void Go_Normal(const float Velocity, const Go_Type GoType)
{
	//Debug �zenetek
	Do_Send_ValueMessageArray();

	//sebess�gbe�ll�t�s
	//ha nincs t�vir�nyit�s v�szle�ll�t�s
	if(Is_StopCommand() == 0)
		MotorControlSetVelocityRef(Velocity);

	//sebess�gszab�lyoz�s
	Do_MotorControl();

	/* Ha be van kapcsolva a vonal(Go_WithLine), k�vesse a vonalat is, ha Go_NoLine van, ne legyen vonalszab�lyoz�s. */
	if(GoType == Go_WithLine)
	{
		Do_PositionControl();
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

	///TODO kiszedni
	while(1)
	{
		Go_Normal(0, GoType);
	}

	//el�rt�k a k�v�nt t�vols�got, meg kell �llni
	while((int)Encoder_GetVelocity() != 0)
	{
		Go_Normal(0, GoType);
	}
}

//Alap vonalk�vet�shez tartoz� algoritmusok
static void Do_Normal_TrackElement(const float VelocityRef)
{
	//v�szle�ll�t�s eset�n stop �llapotba rakjuk
	if(Is_StopCommand())
		TrackElement = Finish_E;
	//ha nincs t�vir�nyit�s v�szle�ll�t�s
	else
		//Sebess�gbe�ll�t�s
		MotorControlSetVelocityRef(VelocityRef);

	//Sebess�gszab�lyoz�s
	Do_MotorControl();

	//akad�lykeres�s
	Look_and_Set_TrackElement(Get_StateLineType(), Get_StateWall());

	//Vonalt�pus meghat�roz�s
	Do_LineType();

	//Falt�pus meghat�roz�s
	Do_WallType();

	//Vonalk�vet�s: �llapotteres szab�lyz�
	Do_PositionControl();

	//Sebess�gszab�lyoz�s
	Do_MotorControl();
}

//K�rforgalom algoritmus
static void Do_Roundabout_TrackElement()
{
	Go_Xcm(20, LOW_SPEED, Go_WithLine);
}

//Dron algoritmus
static void Do_Dron_TrackElement(const float VelocityRef)
{
	//am�g messze vagyunk a dr�nt�l
	while(GetFrontSharpDistance() > DRON_DISTANCE)
	{
		Go_Normal(VelocityRef, Go_WithLine);
	}
	//el�tt�nk van a dr�n, �llunk
	while(GetFrontSharpDistance() < NO_DRON_DISTANCE)
	{
		Go_Normal(0, Go_WithLine);
	}
	//min 2 m�sodperc v�rakoz�s
	HAL_Delay(3000);
	//minden �llapotv�ltoz� alap�llapotba
	Set_TrackElement_Normal();
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

//Elindul a robot! Minden �llapotv�ltoz�t alap�llapotba rak
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
