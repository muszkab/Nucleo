/*
 * SpeedControl.c
 *
 *  Created on: 2016. dec. 6.
 *      Author: Muszka
 */

#include "SpeedControl.h"

//egyenesben a sebesség
#define STRAIGHTSPEED 	20
//kanyarban a sebesség
#define CORNERSPEED 	9

//szabályzó paraméterek
#define T_SPEEDCONTROL	3			//T*1ms
#define T_ACCELERATE	100			//T*1ms
#define T_BRAKE			6			//T*1ms

/* Változók */
//állapotváltozó: kanyar vagy egyenes van épp
speedState StateQ1 = Stop;
//egyenesben a sebesség
uint8_t StraigthSpeed = STRAIGHTSPEED;
//kanyarban a sebesség
uint8_t CornerSpeed = CORNERSPEED;
//adott pillanatban a sebesség
uint8_t SpeedNow;

/* Sebesség beállítása fix értékre, a pályaelemek alapján: kanyar vagy egyenes */
void Do_SpeedControl_FixSpeed()
{
	//T_SPEEDCONTROL ms ciklusidõ  biztosítása. TimeSpeedControl: 1ms periódus
	if(TimeSpeedControl > T_SPEEDCONTROL)
	{
		//számláló nullázás a ciklus újrakezdéséhez
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
					//ha nincs vonal álljon meg, és Stop állapot, hogy csak gombnyomásra induljon
					if(LineNumber == NoLine)
					{
						StateQ1 = Stop;
					}
					else
						if(SpeedNow<StraigthSpeed)
							Accelerate(StraigthSpeed);
					break;
		}
		//sebességbeállítás
		SetSpeed(SpeedNow);
	}
}

//SpeedNow értéket felgyorsítja MaxSpeedig
void Accelerate(uint8_t MaxSpeed)
{
	if(TimeAccelerate > T_ACCELERATE)
	{
		TimeAccelerate=0;
		//gyorsítás MaxSpeed-ig
		if(SpeedNow<MaxSpeed)
			SpeedNow++;
	}
}

void Brake(uint8_t MinSpeed)
{
	if(TimeBrake > T_BRAKE)
	{
		TimeBrake=0;
		//lassítás MinSpeedig
		if(SpeedNow>MinSpeed)
			SpeedNow-=3;
	}
}
