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
#include <BSP.h>

/* Itt beállítható, hogy melyik motorvezérlõt használjuk */
#define HBRIDGE_HOMEMADE
#ifdef HBRIDGE_HOMEMADE
	#define Motor_Init() 	Motor_PWM_Init()
#else
	#define Motor_Init()	Motor_PWM_Init_Factory()
#endif /* HBRIDGE_HOMEMADE */

/*****************************************************/
/**************** SAJÁT MOTORVEZÉRLÕ *****************/
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
/**************** GYÁRI MOTORVEZÉRLÕ *****************/
/*****************************************************/
/* Definition for TIMx clock resources */
#define TIM_MOTOR_FACTORY                  		TIM13
#define TIM_MOTOR_FACTORY_CLK_ENABLE()    		__HAL_RCC_TIM13_CLK_ENABLE()

/* Definition for TIMx Channel Pins  */
/* TIM13_CH1:	PA6 */
#define TIM_MOTOR_FACTORY_CHANNEL_GPIO_PORT()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define TIM_MOTOR_FACTORY_GPIO_PORT     		GPIOA
#define TIM_MOTOR_FACTORY_GPIO_PIN      		GPIO_PIN_6
#define TIM_MOTOR_FACTORY_GPIO_AF				GPIO_AF9_TIM13
#define TIM_MOTOR_FACTORY_CHANNEL				TIM_CHANNEL_1

/*Functions*/
void Motor_PWM_Init();
void Motor_PWM_Init_Factory();
void SetSpeed(int16_t Speed);
void SetSpeedFactory(int8_t Speed);
void MotorStop();

#endif /* MOTOR_H_ */
