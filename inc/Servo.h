/*
 * Servo.h
 *
 *  Created on: 2016. okt. 22.
 *      Author: Muszka
 */

#ifndef SERVO_H_
#define SERVO_H_

#include "Nucleo.h"

/*Global variables*/
extern TIM_OC_InitTypeDef 	sConfig_Servo;
extern TIM_HandleTypeDef    TimHandle_Servo;
extern uint8_t 				ServoPos;
/*Functions*/
void Servo_TIM10_PWM_Init();
void SetServoPWMPulse(const int8_t ServoPos);

#endif /* SERVO_H_ */
