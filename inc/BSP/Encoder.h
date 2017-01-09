/*
 * Encoder.h
 *
 *  Created on: 2016. dec. 23.
 *      Author: szabgyul92
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BSP_ENCODER_H_
#define BSP_ENCODER_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_msp.h"

/**********************************************/
/************ Encoder Handle Timer ************/
/**********************************************/
/* Definition for Encoder timer */
#define ENC_TIM 						TIM2

/* Definition for Encoder clock resources */
#define ENC_TIM_CLK_ENABLE()			__HAL_RCC_TIM2_CLK_ENABLE();
#define ENC_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOA_CLK_ENABLE();

/* Definition for Encoder Pins */
#define ENC_TIM_GPIO_PIN_CHANNEL1 		GPIO_PIN_0
#define ENC_TIM_GPIO_PIN_CHANNEL2   	GPIO_PIN_1

#define ENC_GPIO_PORT 					GPIOA

/**********************************************/
/************ Encoder Callback Timer ************/
/**********************************************/
/* Definition for Encoder Callback timer */
#define ENC_CALLBACK_TIM 				TIM9

/* Definition for Encoder clock resources */
#define ENC_CALLBACK_TIM_CLK_ENABLE()	__HAL_RCC_TIM9_CLK_ENABLE();

/* Variables */
/* Encoder Timer handler declaration */
TIM_HandleTypeDef    Encoder_Handle;
/* Timer Encoder Configuration Structure declaration */
TIM_Encoder_InitTypeDef sEncoderConfig;

/* Global variables */
/* Encoder Callback Timer handler declaration */
extern TIM_HandleTypeDef    Encoder_Callback_Handle;

/*Functions*/
void Encoder_Init();
void Encoder_Callback_Timer_Init();
void Encoder_Timer_Init();
void Encoder_Callback_Timer();

float Encoder_GetDistance();     	// m
float Encoder_GetDistance_cm();	 	// cm
float Encoder_GetVelocity(void); 	// m/s
float Encoder_GetVelocityRaw(void);	// inkrement/s
void Encoder_Reset();

#endif /* BSP_ENCODER_H_ */
