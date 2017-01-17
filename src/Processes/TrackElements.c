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
//TODO: átgondolni a rendszert, include-ok vagy 'Do_X'fgv-ek a processes.h-ban?

/* Változók */
//milyen akadályt teljesít épp
static State_TrackElement TrackElement;

/*  Static függvények */
static void Do_Normal_TrackElement();
static void Look_and_Set_TrackElement(State_LineType StateLineType, State_Wall StateWall);

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
			break;
	}

}

//Alap vonalkövetéshez tartozó algoritmusok
static void Do_Normal_TrackElement()
{
	//akadálykeresés
	//Look_and_Set_TrackElement(Get_StateLineType(), Get_StateWall());

	//Vonaltípus meghatározás
	Do_LineType();

	//Faltípus meghatározás
	Do_WallType();

	//Vonalkövetés: Állapotteres szabályzó
	Do_PositionControl_AT();

	//Sebesség: fix érték vonaltípus alapján, Q1
	//Do_SpeedControl_FixSpeed();
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
