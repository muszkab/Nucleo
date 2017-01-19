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

/* IDENTIFIKÁCIÓ PARAMÉTEREK
 * pozitív tartományra mûködik
 * a CYCLE egész számra jöjjön ki */
#define START_VALUE 	50	   		//a mérés kezdeti pontja
#define END_VALUE 		50	   		//a mérés végpontja, a beavatkozó szerv telítésének közelében
#define STEP 			1	   		//ebben a lépésközben fogja inkrementálni a beavatkozó jelet
#define CYCLE			(END_VALUE - START_VALUE) / STEP
#define TIME_RUN		3000   		//ms
#define TIME_STEP_RUN	25     		//ennyi idõközönként küld adatot a bluetoothon
#define TIME_WAIT		3000   	   	//ms
#define TIME_STEP_WAIT  25

/* Global variables */
extern uint8_t IdentificationEnable;

/* Külsõ változók */
//Motor speed control segédszámláló
extern uint16_t TimeMotorControl;

/* Külsõ függvények */
extern void SetSpeed(int16_t Speed);	//motor.c

/* Saját függvények */
void Do_MotorControl();
void MotorControlSetVelocityRef(float Vref);
float MotorControlGetVelocityRef();
float MotorControlSaturate(float u);
float LookUpTable(float u);
void MotorIdentification();

#endif /* PROCESSES_MOTORCONTROL_H_ */
