/*
 * SpeedControl.h
 *
 *  Created on: 2016. dec. 6.
 *      Author: Muszka
 */

#ifndef SPEEDCONTROL_H_
#define SPEEDCONTROL_H_

#include "Processes.h"

/* K�ls� v�ltoz�k */
//sebess�gszab�lyoz�s seg�dsz�ml�l� a ciklusid�h�z
extern uint16_t TimeSpeedControl;
//gyors�t�shoz seg�dsz�ml�l�
extern uint16_t TimeAccelerate;
//lass�t�shoz seg�dsz�ml�l�
extern uint16_t TimeBrake;

/* K�ls� f�ggv�nyek */
void SetSpeed(int16_t Speed);	//Motor.c

/* Saj�t v�ltoz�k */
//�llapotv�ltoz�: kanyar vagy egyenes van �pp
extern speedState StateQ1;
//adott pillanatban a sebess�g
extern uint8_t SpeedNow;
//egyenesben a sebess�g
extern uint8_t StraigthSpeed;
//kanyarban a sebess�g
extern uint8_t CornerSpeed;


/* Publikus saj�t f�ggv�nyek */
/* Q1 csak: Sebess�g be�ll�t�sa fix �rt�kre, a p�lyaelemek alapj�n: kanyar vagy egyenes */
void Do_SpeedControl_FixSpeed();
//legyen static!
void Accelerate(uint8_t MaxSpeed);
void Brake(uint8_t MinSpeed);


#endif /* SPEEDCONTROL_H_ */
