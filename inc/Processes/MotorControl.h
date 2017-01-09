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

/* IDENTIFIKÁCIÓ PARAMÉTEREK
 * pozitív tartományra mûködik
 * a CYCLE egész számra jöjjön ki */
#define START_VALUE 	0	   		//a mérés kezdeti pontja
#define END_VALUE 		30	   		//a mérés végpontja, a beavatkozó szerv telítésének közelében
#define STEP 			5	   		//ebben a lépésközben fogja inkrementálni a beavatkozó jelet
#define CYCLE			(END_VALUE - START_VALUE) / STEP
#define TIME_RUN		5000   		//ms
#define TIME_STEP_RUN	50     		//ennyi idõközönként küld adatot a bluetoothon
#define TIME_WAIT		5000   	   	//ms
#define TIME_STEP_WAIT  50

/* Global variables */
extern uint8_t IdentificationEnable;

void MotorControl(float VelocityRef);
float MotorControlSaturate(float u);
float LookUpTable(float u);
void MotorIdentification();
#endif /* PROCESSES_MOTORCONTROL_H_ */
