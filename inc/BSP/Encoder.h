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

/* Definition for Encoder timer */
#define ENC_TIM 					TIM2

/* Definition for Encoder clock resources */
#define ENC_TIM_CLK_ENABLE()		__HAL_RCC_TIM2_CLK_ENABLE();
#define ENC_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE();

/* Definition for Encoder Pins */
#define ENC_TIM_GPIO_PIN_CHANNEL1 	GPIO_PIN_0
#define ENC_TIM_GPIO_PIN_CHANNEL2   GPIO_PIN_1

#define ENC_GPIO_PORT 					GPIOA

/*Functions*/
void Encoder_Init();

#endif /* BSP_ENCODER_H_ */
