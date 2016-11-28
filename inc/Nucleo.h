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

/*Global variables*/
extern uint8_t 		ServoPos;

/*Functions*/
void Error_Handler(void);
void Error_SendUart(char* string);
void ProcessMessage(char* message);
void UART1_SendStringBlocking(char* string);

#endif /* NUCLEO_H_ */
