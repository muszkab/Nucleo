/*
 * SpeedControl.c
 *
 *  Created on: 2016. dec. 6.
 *      Author: Muszka
 */

#include "SpeedControl.h"

#define STRAIGHTSPEED 	29	//egyenesben a sebesség
#define CORNERSPEED_IN 	7	//kanyarbejáratban a sebesség
#define CORNERSPEED_OUT 10	//kanyarkijáratban a sebesség
//TODO: kanyarbejáraton(7) és kijároton(9) kül. kanyarsebesség

/*
 * T_ACCELERATE > T_SPEEDCONTROL és T_BRAKE > T_SPEEDCONTROL mindenképp, ha kisebbre van véve akkor is,
 * mert a SpeedControl függvényen belül futnak le a lassító és gyorstító függvények.
 * Sõt, egész számú többszöröse kell legyen T_SPEEDCONTROL-nek.
 */
#define T_SPEEDCONTROL	10		//1ms az egység
#define T_ACCELERATE	(5	*T_SPEEDCONTROL)
#define T_BRAKE			(1	*T_SPEEDCONTROL)
//egy ciklusban hány egységgel változzon a sebesség
#define ACCELERATE_UNIT	1
#define BRAKE_UNIT		3

/* Változók */
//állapotváltozó: kanyar vagy egyenes van épp
speedState StateQ1 = Stop;
//egyenesben a sebesség
uint8_t StraigthSpeed = STRAIGHTSPEED;
//kanyarban a sebesség
uint8_t CornerSpeed = CORNERSPEED_IN;
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

			case(CornerIn):
					CornerSpeed=CORNERSPEED_IN;
					if(SpeedNow>CornerSpeed)
						Brake(CornerSpeed);
					if(SpeedNow<CornerSpeed)
						Accelerate(CornerSpeed);
					break;

			case(CornerOut):
					CornerSpeed=CORNERSPEED_OUT;
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
			SpeedNow+=ACCELERATE_UNIT;
	}
}

void Brake(uint8_t MinSpeed)
{
	if(TimeBrake > T_BRAKE)
	{
		TimeBrake=0;
		//lassítás MinSpeedig
		if(SpeedNow>MinSpeed)
			SpeedNow-=BRAKE_UNIT;
	}
}
