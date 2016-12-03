/*
 * Timers.h
 *
 *  Created on: 2016. nov. 1.
 *      Author: Muszka
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include "Nucleo.h"

#define TIM_SZABALYZO_IRQHandler				TIM6_DAC_IRQHandler

/* V�ltoz�k */
extern uint32_t Szabcount;			//szab�lyz� seg�dsz�ml�l�
extern uint16_t countHalfPeriod;	//f�lperi�dus id� m�r�s az enk�dern�l
extern TIM_HandleTypeDef TimHandle_Szabalyzo;

/* F�ggv�nyek */
void Szabalyzo_TIM_Init();

#endif /* TIMERS_H_ */
