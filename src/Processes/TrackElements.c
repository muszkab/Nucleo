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

/* Változók */
//állapotváltozó, milyen akadályt teljesít épp
static State_TrackElement TrackElement;

/*  Static függvények */
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

/* Csak megy a megadott sebességgel, és  követi a vonalat, ha azt kértük.
 * Do_PositionControl_AT()-val és Do_MotorControl()-val implementálva */
static void Go_Normal(const float Velocity, const Go_Type GoType)
{
	MotorControlSetVelocityRef(Velocity);
	Do_MotorControl();
	//ha be van kapcsolva a vonal, kövesse a vonalat is, egyébként meg ne
	if(GoType == Go_WithLine)
	{
		Do_PositionControl_AT();
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

	//elértük a kívánt távolságot, meg kell állni
	while((int)Encoder_GetVelocity() != 0)
	{
		Go_Normal(0, GoType);
	}
}

//Alap vonalkövetéshez tartozó algoritmusok
static void Do_Normal_TrackElement()
{
	//akadálykeresés
	Look_and_Set_TrackElement(Get_StateLineType(), Get_StateWall());

	//Vonaltípus meghatározás
	Do_LineType();

	//Faltípus meghatározás
	Do_WallType();

	//Vonalkövetés: Állapotteres szabályzó
	Do_PositionControl_AT();

	//Sebesség: fix érték vonaltípus alapján, Q1
	//Do_SpeedControl_FixSpeed();
}

//Körforgalom algoritmus
static void Do_Roundabout_TrackElement()
{
	Go_Xcm(20, 1, Go_WithLine);
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

//minden állapotváltozót alapállapotba rak
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
