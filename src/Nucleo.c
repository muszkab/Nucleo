/*
 * Nucleo.c
 *
 *  Created on: 2016. nov. 2.
 *      Author: Muszka
 */

#include "Nucleo.h"

void Error_Handler(void)
{
  /* Turn LED2 on */
  BSP_LED_On(LED2);
  while (1)
  {
  }
}

void Error_SendUart(char* string)
{
	BSP_LED_On(LED2);
	UART1_SendStringBlocking(string);
}

void ProcessMessage(char* message)
{
	switch(*message)
	{
	case '+':
		ServoPos += 10;
		break;
	case '-':
		ServoPos -= 10;
		break;
	default:
		break;
	}
}
