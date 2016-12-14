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

/* Definition for USARTx clock resources
 * CABLE - USART2*/
#define USART_CABLE                           USART2
#define USART_CABLE_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE();
#define USART_CABLE_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART_CABLE_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USART_CABLE_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USART_CABLE_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

/* Definition for USARTx Pins
 * CABLE
 * TX: PA2 SB63
 * RX: PA3 SB32 */
#define USART_CABLE_TX_PIN                    GPIO_PIN_2
#define USART_CABLE_TX_GPIO_PORT              GPIOA
#define USART_CABLE_TX_AF                     GPIO_AF7_USART2
#define USART_CABLE_RX_PIN                    GPIO_PIN_3
#define USART_CABLE_RX_GPIO_PORT              GPIOA
#define USART_CABLE_RX_AF                     GPIO_AF7_USART2

/* Definition for USARTx clock resources
 * BlUETOOTH - USART1
 * TX: PB6
 * RX: PA10 */
#define USART_BlUETOOTH                           USART1
#define USART_BlUETOOTH_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE();
#define USART_BlUETOOTH_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART_BlUETOOTH_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

#define USART_BlUETOOTH_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()
#define USART_BlUETOOTH_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()

/* Definition for USARTx Pins
 * BLUETOOTH
 * TX: PB6
 * RX: PA10 */
#define USART_BlUETOOTH_TX_PIN                    GPIO_PIN_6
#define USART_BlUETOOTH_TX_GPIO_PORT              GPIOB
#define USART_BlUETOOTH_TX_AF                     GPIO_AF7_USART1
#define USART_BlUETOOTH_RX_PIN                    GPIO_PIN_10
#define USART_BlUETOOTH_RX_GPIO_PORT              GPIOA
#define USART_BlUETOOTH_RX_AF                     GPIO_AF7_USART1

/* Interrupt */
#define USART_CABLE_IRQn						 USART2_IRQn
#define USART_BlUETOOTH_IRQn					 USART1_IRQn

/*Global variables*/
extern UART_HandleTypeDef UartHandle_Cable;
extern UART_HandleTypeDef UartHandle_Bluetooth;
extern uint8_t UART_rxBuff[1];
/*Functions*/
void UART_Cable_Init();
void UART_Bluetooth_Init();

/* Gyula */
void UART_SendNumberBlocking(uint32_t number, UART_HandleTypeDef *UartHandle);
void UART_SendNumberNonBlocking(uint32_t number, UART_HandleTypeDef *UartHandle);
void UART_SendStringBlocking(char* string, UART_HandleTypeDef *UartHandle);
void UART_SendStringNonBlocking(char* string, UART_HandleTypeDef *UartHandle);
void UART_WaitForReadyToSend();
void UART_RecvStringNonBlocking(UART_HandleTypeDef *UartHandle);
void NumToChar(uint32_t number, unsigned char *result, uint16_t* lenght);

/* Muszka */
/*
HAL_StatusTypeDef UART_Send(uint8_t* data, uint8_t size);
HAL_StatusTypeDef UART_PutC(const char c);
HAL_StatusTypeDef UART_NewLine();
void UART_SendString(const char* string);
void UART_SendInt(const int number);
*/

#endif /* UART_H_ */
