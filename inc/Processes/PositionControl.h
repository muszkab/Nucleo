/*
 * PositionControl.h
 *
 *  Created on: 2016. dec. 1.
 *      Author: Muszka
 */

#ifndef POSITIONCONTROL_H_
#define POSITIONCONTROL_H_

#include "Processes.h"
#include "SpeedControl.h"

/* K�ls� v�ltoz�k */
//els� byte a vonalpoz�ci� (int8_t), a m�sodik a vonalak darabsz�ma(0,1,2,3)
extern uint8_t FrontSensor_Data[8];		//CAN.c
//�llapotvisszacsatol�sos szab�lyz�shoz
extern uint8_t RearSensor_Data[8];		//CAN.c
//vonalszab�lyoz�s seg�dsz�ml�l� a ciklusid�h�z
extern uint16_t TimePositionControl;	//Timers.c

/* K�ls� f�ggv�nyek */
void SetServoPWMPulse(const int8_t ServoPos);	//Servo.c

/* Publikus saj�t f�ggv�nyek */
void Do_PositionControl();
int8_t Get_ServoPosition();
int8_t Get_LinePosition();
float Get_P_Corner();
float Get_P_Straight();
float Get_D();


#endif /* POSITIONCONTROL_H_ */
