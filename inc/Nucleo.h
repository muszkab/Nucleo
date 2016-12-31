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

/*Global variables*/
typedef enum{
	NoLine = 0,
	OneLine = 1,
	TwoLine = 2,
	ThreeLine =3
}lineType;

typedef enum{
	Stop,
	CornerIn,
	CornerOut,
	Straight
}speedState;

/*Functions*/
void Error_Handler(void);
void Error_SendUart(char* string);
void ProcessMessage(char* message);

#endif /* NUCLEO_H_ */
