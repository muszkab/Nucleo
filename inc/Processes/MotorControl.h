/*
 * MotorControl.h
 *
 *  Created on: 2017. jan. 5.
 *      Author: szabgyul92
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PROCESSES_MOTORCONTROL_H_
#define PROCESSES_MOTORCONTROL_H_

/* Includes ------------------------------------------------------------------*/
#include "Processes.h"

/* IDENTIFIK�CI� PARAM�TEREK
 * pozit�v tartom�nyra m�k�dik
 * a CYCLE eg�sz sz�mra j�jj�n ki */
#define START_VALUE 	50	   		//a m�r�s kezdeti pontja
#define END_VALUE 		50	   		//a m�r�s v�gpontja, a beavatkoz� szerv tel�t�s�nek k�zel�ben
#define STEP 			1	   		//ebben a l�p�sk�zben fogja inkrement�lni a beavatkoz� jelet
#define CYCLE			(END_VALUE - START_VALUE) / STEP
#define TIME_RUN		3000   		//ms
#define TIME_STEP_RUN	25     		//ennyi id�k�z�nk�nt k�ld adatot a bluetoothon
#define TIME_WAIT		3000   	   	//ms
#define TIME_STEP_WAIT  25

/* Global variables */
extern uint8_t IdentificationEnable;

/* K�ls� v�ltoz�k */
//Motor speed control seg�dsz�ml�l�
extern uint16_t TimeMotorControl;

/* K�ls� f�ggv�nyek */
extern void SetSpeed(int16_t Speed);	//motor.c

/* Saj�t f�ggv�nyek */
void Do_MotorControl();
void MotorControlSetVelocityRef(float Vref);
float MotorControlGetVelocityRef();
float MotorControlSaturate(float u);
float LookUpTable(float u);
void MotorIdentification();

#endif /* PROCESSES_MOTORCONTROL_H_ */
