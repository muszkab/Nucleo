/*
 * ADC.h
 *
 *  Created on: 2016. dec. 24.
 *      Author: szabgyul92
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BSP_ADC_H_
#define BSP_ADC_H_

/* Includes ------------------------------------------------------------------*/
#include "BSP.h"

/* ADC parameter */
#define ADC_RESOLUTION ADC_RESOLUTION_8B
#define ADC_SAMPLETIME ADC_SAMPLETIME_144CYCLES

/* Definition for ADC1 clock resources */
#define ADC1_CLK_ENABLE()                 __HAL_RCC_ADC1_CLK_ENABLE()
#define DMA2_CLK_ENABLE()                 __HAL_RCC_DMA2_CLK_ENABLE()
#define ADC1_CHANNEL4_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define ADC1_CHANNEL10_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE()
#define ADC1_CHANNEL13_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE()
#define ADC1_CHANNEL14_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE()

/* Definition for ADC1 Channel Pins */
#define ADC1_CHANNEL4_GPIO_PORT           GPIOA
#define ADC1_CHANNEL4_PIN                 GPIO_PIN_4
#define ADC1_CHANNEL10_GPIO_PORT          GPIOC
#define ADC1_CHANNEL10_PIN                GPIO_PIN_0
#define ADC1_CHANNEL13_GPIO_PORT          GPIOC
#define ADC1_CHANNEL13_PIN                GPIO_PIN_3
#define ADC1_CHANNEL14_GPIO_PORT          GPIOC
#define ADC1_CHANNEL14_PIN                GPIO_PIN_4


/* Definition for ADC1 Channels */
#define ADC1_CHANNEL4                     ADC_CHANNEL_4
#define ADC1_CHANNEL10                    ADC_CHANNEL_10
#define ADC1_CHANNEL13                    ADC_CHANNEL_13
#define ADC1_CHANNEL14                    ADC_CHANNEL_14

/* Definition of ADC1 DMA resources */
#define ADC1_DMA_CHANNEL               	  DMA_CHANNEL_0
#define ADC1_DMA_STREAM                   DMA2_Stream0

/**********************************************/
/***************** TIMER - ADC ****************/
/**********************************************/
//Timer for ADC periodical conversion
/* Definition for TIM8 clock resources */
#define ADC_TIM                           TIM8
#define ADC_TIM_CLK_ENABLE()              __HAL_RCC_TIM8_CLK_ENABLE()

/*Global variables*/
/* ADC handler declaration */
extern ADC_HandleTypeDef    Adc1Handle;
extern TIM_HandleTypeDef    Adc1_Tim_Handle;

/*Functions*/
void ADC1_Init();
void ADC1_TIM_Init();

#endif /* BSP_ADC_H_ */
