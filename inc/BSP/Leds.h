/*
 * Leds.h
 *
 *  Created on: 2016. dec. 6.
 *      Author: Muszka
 */

#ifndef LEDS_H_
#define LEDS_H_

#include "Nucleo.h"

/*
 * Blue		- háromvonal	LineType.c
 * Red		- kétvonal		LineType.c
 * Yellow	-
 * Green	-
 */

#define ALL_LED_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOC_CLK_ENABLE()

#define RED_LED_PIN						GPIO_PIN_10
#define RED_LED_PORT					GPIOC
#define RED_LED_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOC_CLK_ENABLE()

#define GREEN_LED_PIN					GPIO_PIN_11
#define GREEN_LED_PORT					GPIOC
#define GREEN_LED_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOC_CLK_ENABLE()

#define BLUE_LED_PIN					GPIO_PIN_14
#define BLUE_LED_PORT					GPIOC
#define BLUE_LED_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOC_CLK_ENABLE()

#define YELLOW_LED_PIN					GPIO_PIN_15
#define YELLOW_LED_PORT					GPIOC
#define YELLOW_LED_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOC_CLK_ENABLE()

typedef enum
{
  Blue = 0,
  Red = 1,
  Yellow = 2,
  Green = 3
}ledType;

/* Függvények */
void Leds_Init();
void Led_On(ledType Led);
void Led_Off(ledType Led);
void LED_Toggle(ledType Led);

#endif /* LEDS_H_ */
