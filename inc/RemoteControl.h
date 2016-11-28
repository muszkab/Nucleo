/*
 * RemoteControl.h
 *
 *  Created on: 2016. nov. 27.
 *      Author: Muszka
 */

#ifndef REMOTECONTROL_H_
#define REMOTECONTROL_H_

#include "Nucleo.h"

#define TIM_REMOTE_IRQHandler            TIM1_CC_IRQHandler

/* Timer handler declaration */
TIM_HandleTypeDef    TimHandle_Remote;
/* Távirányító RC-PWM kitöltési tényezõ */
__IO uint32_t            uwDutyCycle;

/*Functions*/
void RemoteController_Timer_Init();

#endif /* REMOTECONTROL_H_ */
