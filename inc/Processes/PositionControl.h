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
#include "Encoder.h"
#include "math.h"

/* Külsõ változók */
//elsõ byte a vonalpozíció (int8_t), a második a vonalak darabszáma(0,1,2,3)
extern uint8_t FrontSensor_Data[8];		//CAN.c
//állapotvisszacsatolásos szabályzáshoz
extern uint8_t RearSensor_Data[8];		//CAN.c
//vonalszabályozás segédszámláló a ciklusidõhöz
extern uint16_t TimePositionControl;	//Timers.c
//állapotteres vonalszabályozás segédszámláló a ciklusidõhöz
extern uint16_t TimePositionControl_AT;	//Timers.c

/* Külsõ függvények */
void SetServoPWMPulse(const int8_t ServoPos);	//Servo.c

/* Publikus saját függvények */
void Do_PositionControl();
void Do_PositionControl_AT();
int8_t Get_ServoPosition();
int8_t Get_LinePosition();
float Get_P_Corner();
float Get_P_Straight();
float Get_D();
float GetLineOrient();

#endif /* POSITIONCONTROL_H_ */
