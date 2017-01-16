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

/* Definition for TIMx clock resources */
#define TIM_SZABALYZO                           TIM6
#define TIM_SZABALYZO_CLK_ENABLE()              __HAL_RCC_TIM6_CLK_ENABLE()
#define TIM_SZABALYZO_IRQn						TIM6_DAC_IRQn

#define TIM_SZABALYZO_IRQHandler				TIM6_DAC_IRQHandler

/* Változók */
extern uint16_t TimePositionControl;		//vonalszabályozás segédszámláló
extern uint16_t TimePositionControl_AT;     //állapotteres vonalszabályozás segédszámláló
extern uint16_t TimeSpeedControl;			//sebességszabályozás segédszámláló
extern uint16_t TimeLineType;				//vonaltípus meghatározás segédszámláló
extern uint16_t TimeAccelerate;				//gyorsításhoz segédszámláló
extern uint16_t TimeBrake;					//lassításhoz segédszámláló
extern uint16_t TimeSendMessage;			//üzenetküldéshez(bluetooth) segédszámláló
extern uint16_t TimeMotorControl;			//Motor speed control
extern uint16_t TimeWallType;				//faltípus felismerés segédszámláló

extern TIM_HandleTypeDef TimHandle_Szabalyzo;

/* Függvények */
void Szabalyzo_TIM_Init();

#endif /* TIMERS_H_ */
