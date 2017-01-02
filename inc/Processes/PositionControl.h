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

/* V�ltoz�k */
//els� byte a vonalpoz�ci� (int8_t), a m�sodik a vonalak darabsz�ma(0,1,2,3)
extern uint8_t FrontSensor_Data[8];
//k�s�bb
extern uint8_t RearSensor_Data[8];
//hibajel, mostani[0] �s el�z�[1] �rt�k
int8_t LinePosition[2];
//vonalszab�lyoz�s seg�dsz�ml�l�
extern uint16_t TimePositionControl;
//�llapotv�ltoz�: kanyar vagy egyenes van �pp
extern speedState StateQ1;

/* F�ggv�nyek */
void SetServoPWMPulse(const int8_t ServoPos);	//Servo.c
void Do_PositionControl();

#endif /* POSITIONCONTROL_H_ */
