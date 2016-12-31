/*
 * Servo.h
 *
 *  Created on: 2016. okt. 22.
 *      Author: Muszka
 */

#ifndef SERVO_H_
#define SERVO_H_

#include "Nucleo.h"

/* Definition for TIMx clock resources */
#define TIMx                           TIM10
#define TIMx_CLK_ENABLE()              __HAL_RCC_TIM10_CLK_ENABLE()

/* Definition for TIMx Channel Pins */
#define TIMx_CHANNEL_GPIO_PORT()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define TIMx_GPIO_PORT_CHANNEL1        GPIOB
#define TIMx_GPIO_PIN_CHANNEL1         GPIO_PIN_8
#define TIMx_GPIO_AF_CHANNEL1          GPIO_AF3_TIM10

/*Global variables*/
extern TIM_OC_InitTypeDef 	sConfig_Servo;
extern TIM_HandleTypeDef    TimHandle_Servo;
/*Functions*/
void Servo_TIM10_PWM_Init();
void SetServoPWMPulse(const int8_t ServoPos);

#endif /* SERVO_H_ */
