/*
 * Leds.c
 *
 *  Created on: 2016. dec. 6.
 *      Author: Muszka
 */

#include "Leds.h"

GPIO_TypeDef* Leds_PORT[4] = {BLUE_LED_PORT, RED_LED_PORT, YELLOW_LED_PORT, GREEN_LED_PORT};

const uint16_t Leds_PIN[4] = {BLUE_LED_PIN, RED_LED_PIN, YELLOW_LED_PIN, GREEN_LED_PIN};

void Leds_Init()
{
	  GPIO_InitTypeDef  GPIO_InitStruct;

	  /* Enable the GPIO_LED Clock */
	  ALL_LED_GPIO_CLK_ENABLE();

	  /* Configure the GPIO_LED pin */
	  GPIO_InitStruct.Pin = BLUE_LED_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

	  HAL_GPIO_Init(BLUE_LED_PORT, &GPIO_InitStruct);
	  HAL_GPIO_WritePin(BLUE_LED_PORT, BLUE_LED_PIN, GPIO_PIN_RESET);

	  GPIO_InitStruct.Pin = RED_LED_PIN;
	  HAL_GPIO_Init(RED_LED_PORT, &GPIO_InitStruct);
	  HAL_GPIO_WritePin(RED_LED_PORT, RED_LED_PIN, GPIO_PIN_RESET);

	  GPIO_InitStruct.Pin = YELLOW_LED_PIN;
	  HAL_GPIO_Init(YELLOW_LED_PORT, &GPIO_InitStruct);
	  HAL_GPIO_WritePin(YELLOW_LED_PORT, YELLOW_LED_PIN, GPIO_PIN_RESET);

	  GPIO_InitStruct.Pin = GREEN_LED_PIN;
	  HAL_GPIO_Init(GREEN_LED_PORT, &GPIO_InitStruct);
	  HAL_GPIO_WritePin(GREEN_LED_PORT, GREEN_LED_PIN, GPIO_PIN_RESET);


	  //TODO: Blue, yellow: SB48,SB49
}

void Led_On(ledType Led)
{
	HAL_GPIO_WritePin(Leds_PORT[Led], Leds_PIN[Led], GPIO_PIN_SET);
}

void Led_Off(ledType Led)
{
	HAL_GPIO_WritePin(Leds_PORT[Led], Leds_PIN[Led], GPIO_PIN_RESET);
}

void LED_Toggle(ledType Led)
{
  HAL_GPIO_TogglePin(Leds_PORT[Led], Leds_PIN[Led]);
}
