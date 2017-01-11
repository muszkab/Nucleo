/*
 * PositionControl.c
 *
 *  Created on: 2016. dec. 1.
 *      Author: Muszka
 */

#include "PositionControl.h"

//szabályzó paraméterek
#define	P_CORNER	1.5f
#define	P_STRAIGHT	1.0f
#define D			20.0f
#define T			10		//T*1ms

/* static változók */
//beavatkozó jel
static float ServoPosition=0;
//P értéke attól függõen milyen állapotban vagyunk: kanyarban magassabb, egyenesben kisebb P
static float P			= P_CORNER;
static float P_Corner	= P_CORNER;
static float P_Straight	= P_STRAIGHT;
//D tag, jelenleg ugyanaz az érték mindenhol
static float D_All		= D;
//hibajel, mostani[0] és elõzõ[1] érték
static int8_t LinePosition[2];

void Do_PositionControl()
{
	//ciklusidõ ellenõrzés és ha nincs vonal, nincs szabályzás, tartsa az elõzõ kormányszöget
	if((TimePositionControl > T) && (Get_LineNumber() != NoLine))
	{
		//számláló nullázás a ciklus újrakezdéséhez
		TimePositionControl=0;

		//ha kanyarsebességet elérte és már csak egy vonal van, legyen nagyobb P
		if((SpeedNow <= CornerSpeed) && (Get_LineNumber() != ThreeLine))
		{
			P=P_Corner;
		}
		else
		{
			P=P_Straight;
		}

		//hibajel
		//-128..127
		LinePosition[1]=LinePosition[0]; 				//elõzõ hibajel
		LinePosition[0]=(int8_t)FrontSensor_Data[0];	//jelenlegi hibajel

		//PD szabályzó
		ServoPosition = -P*LinePosition[0] - D_All*(LinePosition[0]-LinePosition[1]);

		//telítõdés
		if(ServoPosition>127)
			ServoPosition=127;
		if(ServoPosition<-128)
			ServoPosition=-128;

		//szervo állítása
		SetServoPWMPulse((int8_t)ServoPosition);
	}
}

/* Get függvények */
/* ServoPosition: -128..127 */
int8_t Get_ServoPosition()
{
	return (int8_t)ServoPosition;
}

/* LinePosition:-128..127 */
int8_t Get_LinePosition()
{
	return (int8_t)FrontSensor_Data[0];
}

/*  */
float Get_P_Corner()
{
	return P_Corner;
}

/*  */
float Get_P_Straight()
{
	return P_Straight;
}

/*  */
float Get_D()
{
	return D_All;
}
