/*
 * Motor.h
 *
 *  Created on: 2016. nov. 22.
 *      Author: Muszka
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MOTOR_H_
#define MOTOR_H_

/* Includes ------------------------------------------------------------------*/
#include "Nucleo.h"

/* Itt be�ll�that�, hogy melyik motorvez�rl�t haszn�ljuk */
#define HBRIDGE_HOMEMADE
#ifdef HBRIDGE_HOMEMADE
	#define Motor_Init() 	Motor_PWM_Init_Factory()
#else
	#define Motor_Init()	Motor_PWM_Init()
#endif /* HBRIDGE_HOMEMADE */

/*****************************************************/
/**************** SAJ�T MOTORVEZ�RL� *****************/
/*****************************************************/
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

/*****************************************************/
/**************** GY�RI MOTORVEZ�RL� *****************/
/*****************************************************/
/* Definition for TIMx clock resources */
#define TIM_MOTOR_FACTORY                  		TIM8
#define TIM_MOTOR_FACTORY_CLK_ENABLE()    		__HAL_RCC_TIM8_CLK_ENABLE()

/* Definition for TIMx Channel Pins  */
/* TIM8_CH4:	PC9 */
#define TIM_MOTOR_FACTORY_CHANNEL_GPIO_PORT()  __HAL_RCC_GPIOC_CLK_ENABLE()
#define TIM_MOTOR_FACTORY_GPIO_PORT_CHANNEL     GPIOC
#define TIM_MOTOR_FACTORY_GPIO_PIN_CHANNEL      GPIO_PIN_9
#define TIM_MOTOR_FACTORY_GPIO_AF				GPIO_AF3_TIM8
#define TIM_MOTOR_FACTORY_CHANNEL				TIM_CHANNEL_4

/*Functions*/
void Motor_PWM_Init();
void Motor_PWM_Init_Factory();
void SetSpeed(int8_t Speed);
void SetSpeedFactory(int8_t Speed);
void MotorStop();

#endif /* MOTOR_H_ */
