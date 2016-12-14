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

/* Változók */
//adott pillanatban hány darab vonalat állítunk
extern lineType LineNumber;
//állapotváltozó: kanyar vagy egyenes van épp
extern speedState StateQ1;
//adott pillanatban a sebesség
extern uint8_t SpeedNow;
//egyenesben a sebesség
extern uint8_t StraigthSpeed;
//kanyarban a sebesség
extern uint8_t CornerSpeed;

/* Függvények */
/* Sebesség beállítása fix értékre, a pályaelemek alapján: kanyar vagy egyenes */
void Do_SpeedControl_FixSpeed();
void Accelerate(uint8_t MaxSpeed);
void Brake(uint8_t MinSpeed);


#endif /* SPEEDCONTROL_H_ */
