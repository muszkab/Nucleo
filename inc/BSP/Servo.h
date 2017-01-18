/*
 * Servo.h
 *
 *  Created on: 2016. okt. 22.
 *      Author: Muszka
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SERVO_H_
#define SERVO_H_

/* Includes ------------------------------------------------------------------*/
#include <BSP.h>

/**********************************************/
/*********** Servo - Steering Wheel ***********/
/**********************************************/
/* Definition for TIMx clock resources */
#define TIMx                           TIM10
#define TIMx_CLK_ENABLE()              __HAL_RCC_TIM10_CLK_ENABLE()

/* Definition for TIMx Channel Pins */
#define TIMx_CHANNEL_GPIO_PORT()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define TIMx_GPIO_PORT_CHANNEL1        GPIOB
#define TIMx_GPIO_PIN_CHANNEL1         GPIO_PIN_8
#define TIMx_GPIO_AF_CHANNEL1          GPIO_AF3_TIM10

/**********************************************/
/**************** Servo - Sharp ***************/
/**********************************************/
/* Definition for TIM4 clock resources */
#define SERVO_SHARP_TIM                           TIM4
#define SERVO_SHARP_TIM_CLK_ENABLE()              __HAL_RCC_TIM4_CLK_ENABLE()

/* Definition for TIM4 Channel Pins */
#define TIM_SERVO_SHARP_CHANNEL_GPIO_PORT()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define TIM_SERVO_SHARP_GPIO_PORT_CHANNEL2        GPIOB
#define TIM_SERVO_SHARP_GPIO_PIN_CHANNEL2         GPIO_PIN_7
#define TIM_SERVO_SHARP_GPIO_AF_CHANNEL2          GPIO_AF2_TIM4

/*Global variables*/
extern TIM_OC_InitTypeDef 	sConfig_Servo;
extern TIM_HandleTypeDef    TimHandle_Servo;
extern TIM_HandleTypeDef    TimHandle_Servo_Sharp;

/*Functions*/
void Servo_TIM10_PWM_Init();
void Servo_Sharp_TIM4_PWM_Init();
void SetServoPWMPulse(const int8_t ServoPos);
void SetServoSharpPWMPulse(const int8_t ServoPos);

#endif /* SERVO_H_ */
