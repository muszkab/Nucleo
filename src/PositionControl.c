/*
 * PositionControl.c
 *
 *  Created on: 2016. dec. 1.
 *      Author: Muszka
 */

#include "PositionControl.h"

//szabályzó paraméterek
#define	P	2.0f
#define D	10.0f
#define T	100		//T*0.1ms

void Do_PositionControl()
{
	if((Szabcount % T == 0) && FrontSensor_Data[1]!=0)
	{
		//hibajel
		//-128..127
		LinePosition[1]=LinePosition[0]; 				//elõzõ hibajel
		LinePosition[0]=(int8_t)FrontSensor_Data[0];	//jelenlegi hibajel
		//beavatkozó jel
		float ServoPosition;

		//PD szabályzó
		ServoPosition = -P*LinePosition[0] - D*(LinePosition[0]-LinePosition[1]);

		//telítõdés
		if(ServoPosition>127)
			ServoPosition=127;
		if(ServoPosition<-128)
			ServoPosition=-128;

		//szervo állítása
		SetServoPWMPulse((int8_t)ServoPosition);

		//törölni!
		ServoPos=ServoPosition;
	}
}
