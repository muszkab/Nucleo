/*
 * Nucleo.h
 *
 *  Created on: 2016. nov. 2.
 *      Author: Muszka
 */

#ifndef BSP_H_
#define BSP_H_

#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "Leds.h"


#define METER_PER_INCR 			1.3459e-4f    /* m/inkrement */

/*Functions*/
void Error_Handler(void);
void Error_SendUart(char* string);
void ProcessMessage(char* message);

/* Teszt változók */
extern int8_t ServoPos;

#endif /* BSP_H_ */
