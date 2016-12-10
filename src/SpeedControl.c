/*
 * SpeedControl.c
 *
 *  Created on: 2016. dec. 6.
 *      Author: Muszka
 */

#include "SpeedControl.h"

#define STRAIGHTSPEED 	29	//egyenesben a sebess�g
#define CORNERSPEED_IN 	7	//kanyarbej�ratban a sebess�g
#define CORNERSPEED_OUT 10	//kanyarkij�ratban a sebess�g
//TODO: kanyarbej�raton(7) �s kij�roton(9) k�l. kanyarsebess�g

/*
 * T_ACCELERATE > T_SPEEDCONTROL �s T_BRAKE > T_SPEEDCONTROL mindenk�pp, ha kisebbre van v�ve akkor is,
 * mert a SpeedControl f�ggv�nyen bel�l futnak le a lass�t� �s gyorst�t� f�ggv�nyek.
 * S�t, eg�sz sz�m� t�bbsz�r�se kell legyen T_SPEEDCONTROL-nek.
 */
#define T_SPEEDCONTROL	10		//1ms az egys�g
#define T_ACCELERATE	(5	*T_SPEEDCONTROL)
#define T_BRAKE			(1	*T_SPEEDCONTROL)
//egy ciklusban h�ny egys�ggel v�ltozzon a sebess�g
#define ACCELERATE_UNIT	1
#define BRAKE_UNIT		3

/* V�ltoz�k */
//�llapotv�ltoz�: kanyar vagy egyenes van �pp
speedState StateQ1 = Stop;
//egyenesben a sebess�g
uint8_t StraigthSpeed = STRAIGHTSPEED;
//kanyarban a sebess�g
uint8_t CornerSpeed = CORNERSPEED_IN;
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
			SpeedNow+=ACCELERATE_UNIT;
	}
}

void Brake(uint8_t MinSpeed)
{
	if(TimeBrake > T_BRAKE)
	{
		TimeBrake=0;
		//lass�t�s MinSpeedig
		if(SpeedNow>MinSpeed)
			SpeedNow-=BRAKE_UNIT;
	}
}
