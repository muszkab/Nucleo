/*
 * PositionControl.c
 *
 *  Created on: 2016. dec. 1.
 *      Author: Muszka
 */

#include "PositionControl.h"

//szab�lyz� param�terek
#define	P_CORNER	1.5f	//TODO: m�g k�s�bb kapcsoljon nagyobb P-re
#define	P_STRAIGHT	1.0f
#define D			15.0f
#define T			20		//T*1ms

void Do_PositionControl()
{
	if((TimePositionControl > T) && FrontSensor_Data[1]!=0)
	{
		//sz�ml�l� null�z�s a ciklus �jrakezd�s�hez
		TimePositionControl=0;

		//P �rt�ke att�l f�gg�en milyen �llapotban vagyunk: kanyarban magassabb, egyenesben kisebb P
		static float P = P_CORNER;
		//ha kanyarsebess�get el�rte �s m�r csak egy vonal van, legyen nagyobb P
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
		LinePosition[1]=LinePosition[0]; 				//el�z� hibajel
		LinePosition[0]=(int8_t)FrontSensor_Data[0];	//jelenlegi hibajel
		//beavatkoz� jel
		static float ServoPosition=0;

		//PD szab�lyz�
		ServoPosition = -P*LinePosition[0] - D*(LinePosition[0]-LinePosition[1]);

		//tel�t�d�s
		if(ServoPosition>127)
			ServoPosition=127;
		if(ServoPosition<-128)
			ServoPosition=-128;

		//szervo �ll�t�sa
		SetServoPWMPulse((int8_t)ServoPosition);
	}
}
