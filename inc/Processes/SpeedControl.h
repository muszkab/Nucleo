/*
 * SpeedControl.h
 *
 *  Created on: 2016. dec. 6.
 *      Author: Muszka
 */

#ifndef SPEEDCONTROL_H_
#define SPEEDCONTROL_H_

#include "Nucleo.h"
#include "Leds.h"
#include "Motor.h"
#include "Timers.h"

/* V�ltoz�k */
//adott pillanatban h�ny darab vonalat �ll�tunk
extern lineType LineNumber;
//�llapotv�ltoz�: kanyar vagy egyenes van �pp
extern speedState StateQ1;
//adott pillanatban a sebess�g
extern uint8_t SpeedNow;
//egyenesben a sebess�g
extern uint8_t StraigthSpeed;
//kanyarban a sebess�g
extern uint8_t CornerSpeed;

/* F�ggv�nyek */
/* Sebess�g be�ll�t�sa fix �rt�kre, a p�lyaelemek alapj�n: kanyar vagy egyenes */
void Do_SpeedControl_FixSpeed();
void Accelerate(uint8_t MaxSpeed);
void Brake(uint8_t MinSpeed);


#endif /* SPEEDCONTROL_H_ */
