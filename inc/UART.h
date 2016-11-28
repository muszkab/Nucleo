/*
 * USART.h
 *
 *  Created on: 2016. szept. 25.
 *      Author: Muszka
 */

#ifndef UART_H_
#define UART_H_

#include "Nucleo.h"
#include "stdio.h"

/* Definition for USARTx clock resources */
#define USARTx                           USART2
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_2
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_AF                     GPIO_AF7_USART2
#define USARTx_RX_PIN                    GPIO_PIN_3
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_RX_AF                     GPIO_AF7_USART2

/* Interrupt */
#define USARTx_IRQn						 USART2_IRQn


/*Global variables*/
extern UART_HandleTypeDef UartHandle;
extern uint8_t UART_rxBuff[1];
/*Functions*/
void UART_Init();
/* Gyula */
void UART1_SendNumberBlocking(uint32_t number);
void UART1_SendNumberNonBlocking(uint32_t number);
void UART1_SendStringBlocking(char* string);
void UART1_SendStringNonBlocking(char* string);
void UART1_WaitForReadyToSend();
void UART1_RecvStringNonBlocking();
void NumToChar(uint32_t number, unsigned char *result, uint16_t* lenght);
/* Muszka */
HAL_StatusTypeDef UART_Send(uint8_t* data, uint8_t size);
HAL_StatusTypeDef UART_PutC(const char c);
HAL_StatusTypeDef UART_NewLine();
void UART_SendString(const char* string);
void UART_SendInt(const int number);

#endif /* UART_H_ */
