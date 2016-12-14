/*
 * ADC.h
 *
 *  Created on: 2016. szept. 25.
 *      Author: Muszka
 */

#ifndef ADC_H_
#define ADC_H_

#include "Nucleo.h"

/* Definition for ADC1 clock resources */
#define ADC1_CLK_ENABLE()               __HAL_RCC_ADC1_CLK_ENABLE()
#define ADC1_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define ADC1_FORCE_RESET()              __HAL_RCC_ADC_FORCE_RESET()
#define ADC1_RELEASE_RESET()            __HAL_RCC_ADC_RELEASE_RESET()
/* Definition for ADC1 Channel Pin */
/* Left:  PC3
 * Right: PC4
 * Front: PA4 */
#define ADC1_CHANNEL1_PIN               GPIO_PIN_3
#define ADC1_CHANNEL2_PIN				GPIO_PIN_4
#define ADC1_CHANNEL3_PIN				GPIO_PIN_4
#define ADC1_CHANNEL1_GPIO_PORT    		GPIOC
#define ADC1_CHANNEL2_GPIO_PORT    		GPIOC
#define ADC1_CHANNEL3_GPIO_PORT   		GPIOA
/* Definition for ADC1's Channel */
#define ADC1_CHANNEL1                   ADC_CHANNEL_13
#define ADC1_CHANNEL2					ADC_CHANNEL_14
#define ADC1_CHANNEL3					ADC_CHANNEL_15
/* Definition for ADC1's NVIC */
#define ADC1_IRQn                       ADC_IRQn

/*Global variables*/
extern ADC_HandleTypeDef    AdcHandle;
volatile uint16_t uhADCxConvertedValue;
/*Functions*/
void ADC1_Init();
void TimerADC_Init();

#endif /* ADC_H_ */
