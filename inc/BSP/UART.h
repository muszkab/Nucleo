/*
 * USART.h
 *
 *  Created on: 2016. szept. 25.
 *      Author: Muszka
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef UART_H_
#define UART_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_msp.h"
#include "Nucleo.h"
#include "stdio.h"

/**********************************************/
/*************** CABLE - USART2 ***************/
/**********************************************/
/* Definition for CABLE - USART2 clock resources */
#define USART_CABLE                           USART2
#define USART_CABLE_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE();
#define USART_CABLE_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART_CABLE_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

/* Definition for CABLE - USART2 Pins
 * TX: PA2 SB63
 * RX: PA3 SB32 */
#define USART_CABLE_TX_PIN                    GPIO_PIN_2
#define USART_CABLE_TX_GPIO_PORT              GPIOA
#define USART_CABLE_TX_AF                     GPIO_AF7_USART2
#define USART_CABLE_RX_PIN                    GPIO_PIN_3
#define USART_CABLE_RX_GPIO_PORT              GPIOA
#define USART_CABLE_RX_AF                     GPIO_AF7_USART2

/* Interrupt */
#define USART_CABLE_IRQn						 USART2_IRQn

/**********************************************/
/************* BLUETOOTH - USART1 *************/
/**********************************************/
/* Definition for BLUETOOTH - USART1 clock resources */
#define USART_BlUETOOTH                           USART1
#define USART_BlUETOOTH_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE();
#define USART_BlUETOOTH_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART_BlUETOOTH_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

/* Definition for BLUETOOTH - USART1 Pins
 * TX: PB6
 * RX: PA10 */
#define USART_BlUETOOTH_TX_PIN                    GPIO_PIN_6
#define USART_BlUETOOTH_TX_GPIO_PORT              GPIOB
#define USART_BlUETOOTH_TX_AF                     GPIO_AF7_USART1
#define USART_BlUETOOTH_RX_PIN                    GPIO_PIN_10
#define USART_BlUETOOTH_RX_GPIO_PORT              GPIOA
#define USART_BlUETOOTH_RX_AF                     GPIO_AF7_USART1

/* Interrupt */
#define USART_BlUETOOTH_IRQn					 USART1_IRQn

/**********************************************/
/*************** RADIO - UART5 ***************/
/**********************************************/
/* Definition for RADIO - UART5 clock resources */
#define USART_RADIO                      	  UART5
#define USART_RADIO_CLK_ENABLE()          	  __HAL_RCC_UART5_CLK_ENABLE();
#define USART_RADIO_RX_GPIO_CLK_ENABLE()  	  __HAL_RCC_GPIOD_CLK_ENABLE()
#define USART_RADIO_TX_GPIO_CLK_ENABLE()  	  __HAL_RCC_GPIOC_CLK_ENABLE()

/* Definition for RADIO - UART5 Pins
 * TX: PC12
 * RX: PD2 */
#define USART_RADIO_TX_PIN                	  GPIO_PIN_12
#define USART_RADIO_TX_GPIO_PORT          	  GPIOC
#define USART_RADIO_TX_AF                	  GPIO_AF8_UART5
#define USART_RADIO_RX_PIN                	  GPIO_PIN_2
#define USART_RADIO_RX_GPIO_PORT          	  GPIOD
#define USART_RADIO_RX_AF                 	  GPIO_AF8_UART5

/* Interrupt */
#define USART_RADIO_IRQn					  UART5_IRQn

/*Global variables*/
extern UART_HandleTypeDef UartHandle_Cable;
extern UART_HandleTypeDef UartHandle_Bluetooth;
extern UART_HandleTypeDef UartHandle_Radio;
extern uint8_t UART_rxBuff[1];

/*Functions*/
/* Init */
void UART_Cable_Init();
void UART_Bluetooth_Init();
void UART_Radio_Init();

/* Gyula */
void UART_SendNumberBlocking(uint32_t number, UART_HandleTypeDef *UartHandle);
void UART_SendNumberNonBlocking(uint32_t number, UART_HandleTypeDef *UartHandle);
void UART_SendStringBlocking(char* string, UART_HandleTypeDef *UartHandle);
void UART_SendStringNonBlocking(char* string, UART_HandleTypeDef *UartHandle);
void UART_WaitForReadyToSend();
void UART_RecvStringNonBlocking(UART_HandleTypeDef *UartHandle);

void NumToChar(uint32_t number, unsigned char *result, uint16_t* lenght);

//TODO ezek még kellenek?
/* Muszka */
/*
HAL_StatusTypeDef UART_Send(uint8_t* data, uint8_t size);
HAL_StatusTypeDef UART_PutC(const char c);
HAL_StatusTypeDef UART_NewLine();
void UART_SendString(const char* string);
void UART_SendInt(const int number);
*/

#endif /* UART_H_ */
