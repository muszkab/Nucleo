/*
 * PositionControl.h
 *
 *  Created on: 2016. dec. 1.
 *      Author: Muszka
 */

#ifndef POSITIONCONTROL_H_
#define POSITIONCONTROL_H_

#include "Nucleo.h"

/* Változók */
//elsõ byte a vonalpozíció (int8_t), a második a vonalak darabszáma(0,1,2,3)
extern uint8_t FrontSensor_Data[8];
//késõbb
extern uint8_t RearSensor_Data[8];
//beavatkozó jel
extern uint8_t ServoPos;
//hibajel, mostani[0] és elõzõ[1] érték
int8_t LinePosition[2];
//szabályzó segédszámláló
extern uint32_t Szabcount;

/*Függvények  */
void SetServoPWMPulse(const int8_t ServoPos);
void Do_PositionControl();

#endif /* POSITIONCONTROL_H_ */
