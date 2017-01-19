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

/* Akadály paraméterek */
#define LOW_SPEED			0.6f	//alacsony sebesség
#define DRON_DISTANCE		30
#define NO_DRON_DISTANCE	100

//Pozíció szabályozás: PD vagy állapotteres TODO: PD behangolás!
//#define Do_PositionControl()	Do_PositionControl_PD()
#define Do_PositionControl()	Do_PositionControl_AT()

/* Változók */
//állapotváltozó, milyen akadályt teljesít épp
static State_TrackElement TrackElement = Finish_E;

/*  Static függvények */
/* Akadályok */
static void Do_Normal_TrackElement(const float VelocityRef);
static void Do_Roundabout_TrackElement();
static void Do_Dron_TrackElement(const float VelocityRef);
/* Segédfüggvények */
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

/* Csak megy a megadott sebességgel, és  követi a vonalat, ha azt kértük.
 * Do_PositionControl()-val és Do_MotorControl()-val implementálva */
static void Go_Normal(const float Velocity, const Go_Type GoType)
{
	//Debug üzenetek
	Do_Send_ValueMessageArray();

	//sebességbeállítás
	//ha nincs távirányitós vészleállítás
	if(Is_StopCommand() == 0)
		MotorControlSetVelocityRef(Velocity);

	//sebességszabályozás
	Do_MotorControl();

	/* Ha be van kapcsolva a vonal(Go_WithLine), kövesse a vonalat is, ha Go_NoLine van, ne legyen vonalszabályozás. */
	if(GoType == Go_WithLine)
	{
		Do_PositionControl();
	}
}

/* Annyi centimétert megy elõre(pozitív) vagy hátra(negatív), amennyit paraméterben megkap.
 * Sebességet is lehet állítani paraméterben.
 * TotalDistance: [cm] pozitív és negatív(elõre-hátra)
 * Velocity: [m/s] csak pozitív
 */
static void Go_Xcm(const float TotalDistance, const float Velocity, const Go_Type GoType)
{
	//ha a sebesség negatív, ne csináljon semmit
	if(Velocity < 0)
		return;

	//abszolút pont lekérése, ehhez fogunk viszonyítani
	float StartPos = Encoder_GetDistance_cm();
	//megtett távolság jelenleg
	float CurrentDistance = 0;
	//mekkora sebességgel menjen, ha TotalDistance negatív, ez is negatív kell legyen
	float t_velocity;

	//ha hátrafelé kell menni, negatív sebességet kell a szabályzónak beállítani
	if(TotalDistance < 0)
	{
		t_velocity = -Velocity;
	}
	else
	{
		//pozitív sebességgel indítás
		t_velocity = Velocity;
	}

	//amíg nem tette meg a kívánt távolságot
	while(abs(CurrentDistance) < abs(TotalDistance))
	{
		//Vonalkövetés
		Go_Normal(t_velocity, GoType);
		//megtett út kiszámítása
		CurrentDistance = Encoder_GetDistance_cm() - StartPos;
	}

	///TODO kiszedni
	while(1)
	{
		Go_Normal(0, GoType);
	}

	//elértük a kívánt távolságot, meg kell állni
	while((int)Encoder_GetVelocity() != 0)
	{
		Go_Normal(0, GoType);
	}
}

//Alap vonalkövetéshez tartozó algoritmusok
static void Do_Normal_TrackElement(const float VelocityRef)
{
	//vészleállítás esetén stop állapotba rakjuk
	if(Is_StopCommand())
		TrackElement = Finish_E;
	//ha nincs távirányitós vészleállítás
	else
		//Sebességbeállítás
		MotorControlSetVelocityRef(VelocityRef);

	//Sebességszabályozás
	Do_MotorControl();

	//akadálykeresés
	Look_and_Set_TrackElement(Get_StateLineType(), Get_StateWall());

	//Vonaltípus meghatározás
	Do_LineType();

	//Faltípus meghatározás
	Do_WallType();

	//Vonalkövetés: Állapotteres szabályzó
	Do_PositionControl();

	//Sebességszabályozás
	Do_MotorControl();
}

//Körforgalom algoritmus
static void Do_Roundabout_TrackElement()
{
	Go_Xcm(20, LOW_SPEED, Go_WithLine);
}

//Dron algoritmus
static void Do_Dron_TrackElement(const float VelocityRef)
{
	//amíg messze vagyunk a dróntól
	while(GetFrontSharpDistance() > DRON_DISTANCE)
	{
		Go_Normal(VelocityRef, Go_WithLine);
	}
	//elõttünk van a drón, állunk
	while(GetFrontSharpDistance() < NO_DRON_DISTANCE)
	{
		Go_Normal(0, Go_WithLine);
	}
	//min 2 másodperc várakozás
	HAL_Delay(3000);
	//minden állapotváltozó alapállapotba
	Set_TrackElement_Normal();
}
/* figyeli az akadályt jelzõ állapotváltozókat(StateLineType és StateWall),
 * és azoknak megfelelõen állítja a TrackElement-et, ami mutatja milyen akadály van épp
 */
static void Look_and_Set_TrackElement(State_LineType StateLineType, State_Wall StateWall)
{
	//vonallal jelzett akadályok
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
	//fallal jelzett akadályok
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

//Elindul a robot! Minden állapotváltozót alapállapotba rak
void Set_TrackElement_Normal()
{
	//pályaelem
	TrackElement = Normal_E;
	//vonaltípus
	Set_StateLineType(Egyvonal_folyt);
	//faltípus
	Set_StateWall(Nincsfal);
}

/* Get függvények */
State_TrackElement Get_TrackElement()
{
	return TrackElement;
}
