/*
 * Leds.h
 *
 *  Created on: 2016. dec. 6.
 *      Author: Muszka
 */

#ifndef LEDS_H_
#define LEDS_H_

#include "Nucleo.h"

#define ALL_LED_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOC_CLK_ENABLE()

#define BLUE_LED_PIN				GPIO_PIN_10
#define BLUE_LED_PORT				GPIOC
#define BLUE_LED_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOC_CLK_ENABLE()

#define RED_LED_PIN					GPIO_PIN_11
#define RED_LED_PORT				GPIOC
#define RED_LED_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOC_CLK_ENABLE()

#define YELLOW_LED_PIN					GPIO_PIN_14
#define YELLOW_LED_PORT					GPIOC
#define YELLOW_LED_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOC_CLK_ENABLE()

#define YELLOW2_LED_PIN					GPIO_PIN_15
#define YELLOW2_LED_PORT				GPIOC
#define YELLOW2_LED_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOC_CLK_ENABLE()

/* Függvények */
void Leds_Init();
void Led_On(ledType Led);
void Led_Off(ledType Led);
void LED_Toggle(ledType Led);

#endif /* LEDS_H_ */
