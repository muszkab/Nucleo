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

void MotorControl(float VelocityRef);
float MotorControlSaturate(float u);
float LookUpTable(float u);

#endif /* PROCESSES_MOTORCONTROL_H_ */
