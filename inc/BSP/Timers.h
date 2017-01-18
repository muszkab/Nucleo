/*
 * Timers.h
 *
 *  Created on: 2016. nov. 1.
 *      Author: Muszka
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include "Nucleo.h"
#include "Encoder.h"

/*********************************/
/******** SZAB�LYZ� TIMER ********/
/*********************************/
/* Definition for TIMx clock resources */
#define TIM_SZABALYZO                           TIM6
#define TIM_SZABALYZO_CLK_ENABLE()              __HAL_RCC_TIM6_CLK_ENABLE()
#define TIM_SZABALYZO_IRQn						TIM6_DAC_IRQn

#define TIM_SZABALYZO_IRQHandler				TIM6_DAC_IRQHandler

/****************************/
/******** GYRO TIMER ********/
/****************************/
#define GYRO_TIM_FREQ		10000
#define GYRO_TIM_PERIOD	    5000
/* Definition for TIMx clock resources */
#define TIM_GYRO                          TIM7
#define TIM_GYRO_CLK_ENABLE()             __HAL_RCC_TIM7_CLK_ENABLE()
#define TIM_GYRO_IRQn					  TIM7_IRQn
#define TIM_GYRO_IRQHandler 			  TIM7_IRQHandler

/* V�ltoz�k */
extern uint16_t TimePositionControl;		//vonalszab�lyoz�s seg�dsz�ml�l�
extern uint16_t TimePositionControl_AT;     //�llapotteres vonalszab�lyoz�s seg�dsz�ml�l�
extern uint16_t TimeSpeedControl;			//sebess�gszab�lyoz�s seg�dsz�ml�l�
extern uint16_t TimeLineType;				//vonalt�pus meghat�roz�s seg�dsz�ml�l�
extern uint16_t TimeAccelerate;				//gyors�t�shoz seg�dsz�ml�l�
extern uint16_t TimeBrake;					//lass�t�shoz seg�dsz�ml�l�
extern uint16_t TimeSendMessage;			//�zenetk�ld�shez(bluetooth) seg�dsz�ml�l�
extern uint16_t TimeMotorControl;			//Motor speed control
extern uint16_t TimeWallType;				//falt�pus felismer�s seg�dsz�ml�l�

extern TIM_HandleTypeDef TimHandle_Szabalyzo;
extern TIM_HandleTypeDef TimHandle_Gyro;

/* F�ggv�nyek */
void Szabalyzo_TIM_Init();
void GYRO_TIM_Init();

#endif /* TIMERS_H_ */
