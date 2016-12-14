/*
 * Encoder.h
 *
 *  Created on: 2016. okt. 26.
 *      Author: Muszka
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include "Nucleo.h"

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
