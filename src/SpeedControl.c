/*
 * SpeedControl.c
 *
 *  Created on: 2016. dec. 6.
 *      Author: Muszka
 */

#include "SpeedControl.h"

//egyenesben a sebess�g
#define STRAIGHTSPEED 	20
//kanyarban a sebess�g
#define CORNERSPEED 	9

//szab�lyz� param�terek
#define T_SPEEDCONTROL	3			//T*1ms
#define T_ACCELERATE	100			//T*1ms
#define T_BRAKE			6			//T*1ms

/* V�ltoz�k */
//�llapotv�ltoz�: kanyar vagy egyenes van �pp
speedState StateQ1 = Stop;
//egyenesben a sebess�g
uint8_t StraigthSpeed = STRAIGHTSPEED;
//kanyarban a sebess�g
uint8_t CornerSpeed = CORNERSPEED;
//adott pillanatban a sebess�g
uint8_t SpeedNow;

/* Sebess�g be�ll�t�sa fix �rt�kre, a p�lyaelemek alapj�n: kanyar vagy egyenes */
void Do_SpeedControl_FixSpeed()
{
	//T_SPEEDCONTROL ms ciklusid�  biztos�t�sa. TimeSpeedControl: 1ms peri�dus
	if(TimeSpeedControl > T_SPEEDCONTROL)
	{
		//sz�ml�l� null�z�s a ciklus �jrakezd�s�hez
		TimeSpeedControl=0;

		switch(StateQ1){
			case(Stop):
					SpeedNow=0;
					break;

			case(Corner):
					if(SpeedNow>CornerSpeed)
						Brake(CornerSpeed);
					if(SpeedNow<CornerSpeed)
						Accelerate(CornerSpeed);
					break;

			case(Straight):
					//ha nincs vonal �lljon meg, �s Stop �llapot, hogy csak gombnyom�sra induljon
					if(LineNumber == NoLine)
					{
						StateQ1 = Stop;
					}
					else
						if(SpeedNow<StraigthSpeed)
							Accelerate(StraigthSpeed);
					break;
		}
		//sebess�gbe�ll�t�s
		SetSpeed(SpeedNow);
	}
}

//SpeedNow �rt�ket felgyors�tja MaxSpeedig
void Accelerate(uint8_t MaxSpeed)
{
	if(TimeAccelerate > T_ACCELERATE)
	{
		TimeAccelerate=0;
		//gyors�t�s MaxSpeed-ig
		if(SpeedNow<MaxSpeed)
			SpeedNow++;
	}
}

void Brake(uint8_t MinSpeed)
{
	if(TimeBrake > T_BRAKE)
	{
		TimeBrake=0;
		//lass�t�s MinSpeedig
		if(SpeedNow>MinSpeed)
			SpeedNow-=3;
	}
}
