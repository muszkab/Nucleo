/*
 * Motor.h
 *
 *  Created on: 2016. nov. 22.
 *      Author: Muszka
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "Nucleo.h"

/* Definition for TIMx clock resources */
#define TIM_MOTOR                           TIM3
#define TIM_MOTOR_CLK_ENABLE()              __HAL_RCC_TIM3_CLK_ENABLE()

/* Definition for TIMx Channel Pins  */
/* TIM3_CH2:	PA7
 * TIM3_CH1:	PC6 */
#define TIM_MOTOR_CHANNEL1_GPIO_PORT()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define TIM_MOTOR_CHANNEL2_GPIO_PORT()       __HAL_RCC_GPIOC_CLK_ENABLE()
#define TIM_MOTOR_GPIO_PORT_CHANNEL1        GPIOA
#define TIM_MOTOR_GPIO_PORT_CHANNEL2        GPIOC
#define TIM_MOTOR_GPIO_PIN_CHANNEL1         GPIO_PIN_7
#define TIM_MOTOR_GPIO_PIN_CHANNEL2         GPIO_PIN_6
#define TIM_MOTOR_GPIO_AF					GPIO_AF2_TIM3

/*Functions*/
void Motor_PWM_Init();
void SetSpeed(int8_t Speed);
void MotorStop();

#endif /* MOTOR_H_ */
