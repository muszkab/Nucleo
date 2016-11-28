/*
 * Motor.h
 *
 *  Created on: 2016. nov. 22.
 *      Author: Muszka
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "Nucleo.h"

/*Functions*/
void Motor_PWM_Init();
void SetSpeed(int8_t Speed);
void MotorStop();

#endif /* MOTOR_H_ */
