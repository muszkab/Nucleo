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
#include "Encoder.h"
#include "Nucleo.h"
#include "Processes.h"
#include "Timers.h"

/* IDENTIFIK�CI� PARAM�TEREK
 * pozit�v tartom�nyra m�k�dik
 * a CYCLE eg�sz sz�mra j�jj�n ki */
#define START_VALUE 	0	   		//a m�r�s kezdeti pontja
#define END_VALUE 		30	   		//a m�r�s v�gpontja, a beavatkoz� szerv tel�t�s�nek k�zel�ben
#define STEP 			5	   		//ebben a l�p�sk�zben fogja inkrement�lni a beavatkoz� jelet
#define CYCLE			(END_VALUE - START_VALUE) / STEP
#define TIME_RUN		5000   		//ms
#define TIME_STEP_RUN	50     		//ennyi id�k�z�nk�nt k�ld adatot a bluetoothon
#define TIME_WAIT		5000   	   	//ms
#define TIME_STEP_WAIT  50

/* Global variables */
extern uint8_t IdentificationEnable;

void MotorControl(float VelocityRef);
float MotorControlSaturate(float u);
float LookUpTable(float u);
void MotorIdentification();
#endif /* PROCESSES_MOTORCONTROL_H_ */
