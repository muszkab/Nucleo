/*
 * PositionControl.h
 *
 *  Created on: 2016. dec. 1.
 *      Author: Muszka
 */

#ifndef POSITIONCONTROL_H_
#define POSITIONCONTROL_H_

#include "Nucleo.h"
#include "Processes.h"
#include "SpeedControl.h"

/* Változók */
//elsõ byte a vonalpozíció (int8_t), a második a vonalak darabszáma(0,1,2,3)
extern uint8_t FrontSensor_Data[8];
//késõbb
extern uint8_t RearSensor_Data[8];
//hibajel, mostani[0] és elõzõ[1] érték
int8_t LinePosition[2];
//vonalszabályozás segédszámláló
extern uint16_t TimePositionControl;
//állapotváltozó: kanyar vagy egyenes van épp
extern speedState StateQ1;

/* Függvények */
void SetServoPWMPulse(const int8_t ServoPos);	//Servo.c
void Do_PositionControl();

#endif /* POSITIONCONTROL_H_ */
