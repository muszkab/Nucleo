/*
 * Nucleo.h
 *
 *  Created on: 2016. nov. 2.
 *      Author: Muszka
 */

#ifndef NUCLEO_H_
#define NUCLEO_H_

#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "Leds.h"

/*Functions*/
void Error_Handler(void);
void Error_SendUart(char* string);
void ProcessMessage(char* message);

/* Teszt v�ltoz�k */
extern int8_t ServoPos;

#endif /* NUCLEO_H_ */
