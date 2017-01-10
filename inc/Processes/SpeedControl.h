/*
 * SpeedControl.h
 *
 *  Created on: 2016. dec. 6.
 *      Author: Muszka
 */

#ifndef SPEEDCONTROL_H_
#define SPEEDCONTROL_H_

#include "Processes.h"

/* Külsõ változók */
//sebességszabályozás segédszámláló a ciklusidõhöz
extern uint16_t TimeSpeedControl;
//gyorsításhoz segédszámláló
extern uint16_t TimeAccelerate;
//lassításhoz segédszámláló
extern uint16_t TimeBrake;

/* Külsõ függvények */
void SetSpeed(int16_t Speed);	//Motor.c

/* Saját változók */
//állapotváltozó: kanyar vagy egyenes van épp
extern speedState StateQ1;
//adott pillanatban a sebesség
extern uint8_t SpeedNow;
//egyenesben a sebesség
extern uint8_t StraigthSpeed;
//kanyarban a sebesség
extern uint8_t CornerSpeed;


/* Publikus saját függvények */
/* Q1 csak: Sebesség beállítása fix értékre, a pályaelemek alapján: kanyar vagy egyenes */
void Do_SpeedControl_FixSpeed();
//legyen static!
void Accelerate(uint8_t MaxSpeed);
void Brake(uint8_t MinSpeed);


#endif /* SPEEDCONTROL_H_ */
