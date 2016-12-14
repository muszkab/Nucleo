/*
 * Timers.h
 *
 *  Created on: 2016. nov. 1.
 *      Author: Muszka
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include "Nucleo.h"

#define TIM_SZABALYZO_IRQHandler		TIM6_DAC_IRQHandler

/* Változók */
extern uint16_t TimePositionControl;		//vonalszabályozás segédszámláló
extern uint16_t TimeSpeedControl;			//sebességszabályozás segédszámláló
extern uint16_t TimeLineType;				//vonaltípus meghatározás segédszámláló
extern uint16_t TimeSpeedState;				//sebesség állapot(StateQ1) ne változzon x másodpercig segédszámláló
extern uint16_t TimeAccelerate;				//gyorsításhoz segédszámláló
extern uint16_t TimeBrake;					//lassításhoz segédszámláló

extern uint32_t Szabcount;			//szabályzó segédszámláló
extern uint16_t countHalfPeriod;	//félperiódus idõ mérés az enkódernél
extern TIM_HandleTypeDef TimHandle_Szabalyzo;

/* Függvények */
void Szabalyzo_TIM_Init();

#endif /* TIMERS_H_ */
