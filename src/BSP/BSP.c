/*
 * Nucleo.c
 *
 *  Created on: 2016. nov. 2.
 *      Author: Muszka
 */

#include <BSP.h>
#include "UART.h"

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
	UART_SendStringBlocking(string, &UartHandle_Cable);
}

int8_t ServoPos=0;
void ProcessMessage(char* message)
{
	switch(*message)
	{
	case 'a':
		ServoPos += 10;
		break;
	case 'q':
		ServoPos -= 10;
		break;
	case '+':
		ServoPos += 1;
		break;
	case '-':
		ServoPos -= 1;
		break;
	default:
		break;
	}
}
