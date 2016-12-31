/*
 * Encoder.h
 *
 *  Created on: 2016. okt. 26.
 *      Author: Muszka
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include "Nucleo.h"

/* Definition for TIM_ENC clock resources */
#define TIM_ENC                           TIM2
#define TIM_ENC_CLK_ENABLE()              __HAL_RCC_TIM2_CLK_ENABLE()

/* Definition for TIM_ENC Channel Pins */
#define TIM_ENC_CHANNEL_GPIO_PORT()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define TIM_ENC_GPIO_PORT			       GPIOA
#define TIM_ENC_GPIO_PIN_CHANNEL1         GPIO_PIN_0
#define TIM_ENC_GPIO_PIN_CHANNEL2         GPIO_PIN_1
#define TIM_ENC_GPIO_ALTERNATE_FUNCTION   GPIO_AF1_TIM2

#define TIM_Encoder_IRQHandler			  TIM2_IRQHandler
#define TIM_HalfPeriod_IRQHandler		  TIM1_TRG_COM_TIM11_IRQHandler

/*Variables*/
/* Timer handler declaration */
TIM_HandleTypeDef    TimHandle_Encoder;

/* Timer Output Compare Configuration Structure declaration */
TIM_Encoder_InitTypeDef sConfig_Encoder;
TIM_HandleTypeDef    TimHandle_HalfPeriod;

/* Count, speed variables */
/* Enkóder számláló értékekbõl számolt mennyiségek*/
static volatile int32_t count;
static volatile int32_t prevCount;
static volatile int32_t 		speed;
volatile int32_t		distance;
/* Félperiódusidõ mérés */
/* Félperiódus hossza, darabszám */
extern uint16_t		countHalfPeriod;
/* Sebesség: kisebb számérték -> nagyobb sebességérték */
volatile uint16_t		speedHalfPeriod;

/*Functions*/
void Encoder_Init();
void Encoder_ReadCount();
void Encoder_ReadHalfPeriod();
void Encoder_VariablesReset();
void Timer11_Init();

#endif /* ENCODER_H_ */
