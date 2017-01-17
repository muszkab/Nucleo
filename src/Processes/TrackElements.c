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
//TODO: �tgondolni a rendszert, include-ok vagy 'Do_X'fgv-ek a processes.h-ban?

/* V�ltoz�k */
//milyen akad�lyt teljes�t �pp
static State_TrackElement TrackElement;

/*  Static f�ggv�nyek */
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

//Alap vonalk�vet�shez tartoz� algoritmusok
static void Do_Normal_TrackElement()
{
	//akad�lykeres�s
	//Look_and_Set_TrackElement(Get_StateLineType(), Get_StateWall());

	//Vonalt�pus meghat�roz�s
	Do_LineType();

	//Falt�pus meghat�roz�s
	Do_WallType();

	//Vonalk�vet�s: �llapotteres szab�lyz�
	Do_PositionControl_AT();

	//Sebess�g: fix �rt�k vonalt�pus alapj�n, Q1
	//Do_SpeedControl_FixSpeed();
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
