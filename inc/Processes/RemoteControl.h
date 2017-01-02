/*
 * RemoteControl.h
 *
 *  Created on: 2016. nov. 27.
 *      Author: Muszka
 */

#ifndef REMOTECONTROL_H_
#define REMOTECONTROL_H_

#include "Nucleo.h"
#include "Processes.h"

/* Definition for TIMx clock resources */
#define TIM_REMOTE                           TIM1
#define TIM_REMOTE_CLK_ENABLE()              __HAL_RCC_TIM1_CLK_ENABLE()

/* Definition for TIMx Pins */
#define TIM_REMOTE_CHANNEL_GPIO_PORT()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define TIM_REMOTE_GPIO_PORT                GPIOA
#define TIM_REMOTE_GPIO_PIN              	GPIO_PIN_9
#define TIM_REMOTE_GPIO_AF                 	GPIO_AF1_TIM1

/* Definition for TIMx's NVIC */
#define TIM_REMOTE_CC_IRQn                   TIM1_CC_IRQn

#define TIM_REMOTE_IRQHandler                TIM1_CC_IRQHandler

/* Timer handler declaration */
extern TIM_HandleTypeDef    TimHandle_Remote;
/* Távirányító RC-PWM kitöltési tényezõ */
__IO uint32_t            uwDutyCycle;

/*Functions*/
void RemoteController_Timer_Init();
void RemoteController_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void SetSpeed_RemoteControl();
void Is_StopCommand();
void SetSpeed(int8_t Speed);	//motor.c

#endif /* REMOTECONTROL_H_ */
