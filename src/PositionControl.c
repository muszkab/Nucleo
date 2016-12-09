/*
 * PositionControl.c
 *
 *  Created on: 2016. dec. 1.
 *      Author: Muszka
 */

#include "PositionControl.h"

//szabályzó paraméterek
#define	P_CORNER	1.5f	//TODO: még késõbb kapcsoljon nagyobb P-re
#define	P_STRAIGHT	1.0f
#define D			15.0f
#define T			20		//T*1ms

void Do_PositionControl()
{
	if((TimePositionControl > T) && FrontSensor_Data[1]!=0)
	{
		//számláló nullázás a ciklus újrakezdéséhez
		TimePositionControl=0;

		//P értéke attól függõen milyen állapotban vagyunk: kanyarban magassabb, egyenesben kisebb P
		static float P = P_CORNER;
		//ha kanyarsebességet elérte és már csak egy vonal van, legyen nagyobb P
		if(SpeedNow <= CornerSpeed && LineNumber!=ThreeLine)
		{
			P=P_CORNER;
			Led_On(Red);
		}
		else
		{
			P=P_STRAIGHT;
			Led_Off(Red);
		}

		//hibajel
		//-128..127
		LinePosition[1]=LinePosition[0]; 				//elõzõ hibajel
		LinePosition[0]=(int8_t)FrontSensor_Data[0];	//jelenlegi hibajel
		//beavatkozó jel
		static float ServoPosition=0;

		//PD szabályzó
		ServoPosition = -P*LinePosition[0] - D*(LinePosition[0]-LinePosition[1]);

		//telítõdés
		if(ServoPosition>127)
			ServoPosition=127;
		if(ServoPosition<-128)
			ServoPosition=-128;

		//szervo állítása
		SetServoPWMPulse((int8_t)ServoPosition);
	}
}
