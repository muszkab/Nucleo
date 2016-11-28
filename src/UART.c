/*
 * USART.c
 *
 *  Created on: 2016. szept. 25.
 *      Author: Muszka
 */
#include "UART.h"

/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

  /* UART handler declaration */
  UART_HandleTypeDef UartHandle;
  /* Uart receive message buffer */
  uint8_t UART_rxBuff[1]={'0'};

  /**/
  uint32_t UARTMessageNumber=0;

void UART_Init()
{
	  GPIO_InitTypeDef  GPIO_InitStruct;

	  /*##-1- Enable peripherals and GPIO Clocks #################################*/
	  /* Enable GPIO TX/RX clock */
	  USARTx_TX_GPIO_CLK_ENABLE();
	  USARTx_RX_GPIO_CLK_ENABLE();


	  /* Enable USARTx clock */
	  USARTx_CLK_ENABLE();

	  /*##-2- Configure peripheral GPIO ##########################################*/
	  /* UART TX GPIO pin configuration  */
	  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
	  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	  GPIO_InitStruct.Pull      = GPIO_PULLUP;
	  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
	  GPIO_InitStruct.Alternate = USARTx_TX_AF;

	  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

	  /* UART RX GPIO pin configuration  */
	  GPIO_InitStruct.Pin = USARTx_RX_PIN;
	  GPIO_InitStruct.Alternate = USARTx_RX_AF;

	  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

	  HAL_NVIC_SetPriority(USARTx_IRQn,1,0);
	  HAL_NVIC_EnableIRQ(USARTx_IRQn);

	  /*##-3- Configure the UART peripheral ######################################*/
	  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
	  /* UART1 configured as follows:
	      - Word Length = 8 Bits
	      - Stop Bit = One Stop bit
	      - Parity = No parity
	      - BaudRate = 9600 baud
	      - Hardware flow control disabled (RTS and CTS signals) */
	  UartHandle.Instance          = USARTx;

	  UartHandle.Init.BaudRate     = 9600;
	  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
	  UartHandle.Init.StopBits     = UART_STOPBITS_1;
	  UartHandle.Init.Parity       = UART_PARITY_NONE;
	  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	  UartHandle.Init.Mode         = UART_MODE_TX_RX;
	  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

	  if(HAL_UART_Init(&UartHandle) != HAL_OK)
	  {
	    /* Initialization Error */
	    Error_Handler();
	  }

	  /* Output a message on Hyperterminal using printf function */
	  //printf("\n\r UART Printf Example: retarget the C library printf function to the UART\n\r");
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if ( huart->Instance == USART2)	{
		UART1_SendStringNonBlocking((char*)UART_rxBuff);
		UART1_RecvStringNonBlocking();
	}
}

void UART1_SendNumberBlocking(uint32_t number)
{
	unsigned char result[10];
	uint16_t lenght = 0;

	NumToChar(number,result,&lenght);
	//UART1_WaitForReadyToSend();
	if(HAL_UART_Transmit(&UartHandle, result,lenght,1000) != HAL_OK){
		Error_Handler();
	}
}
void UART1_SendNumberNonBlocking(uint32_t number)
{
	unsigned char result[10];
	uint16_t lenght = 0;

	//UART1_WaitForReadyToSend();
	NumToChar(number,result,&lenght);
	if(HAL_UART_Transmit_IT(&UartHandle, result,lenght) != HAL_OK){
		Error_Handler();
	}
}
void UART1_SendStringBlocking(char* string)
{
	uint16_t len = 0;
	char* lstring = string;
	while (*lstring)
	{
		len++;
		lstring++;
	}
	//UART1_WaitForReadyToSend();
	if(HAL_UART_Transmit(&UartHandle,(uint8_t*) string,len,1000) != HAL_OK){
		Error_Handler();
	}
}
void UART1_SendStringNonBlocking(char* string)
{
	uint16_t len = 0;
	char* lstring = string;
	while (*lstring)
	{
		len++;
		lstring++;
	}
	//UART1_WaitForReadyToSend();
	if(HAL_UART_Transmit_IT(&UartHandle,(uint8_t*) string,len) != HAL_OK){
		Error_Handler();
	}
}

void UART1_WaitForReadyToSend()
{
	//while ((UartHandle.State != HAL_UART_STATE_BUSY_RX)
	//		&& (UartHandle.State != HAL_UART_STATE_READY));
	while (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY){}
}

void UART1_RecvStringNonBlocking()
{
	if(HAL_UART_Receive_IT(&UartHandle, UART_rxBuff, sizeof(UART_rxBuff)) != HAL_OK){
		Error_Handler();
	}
	ProcessMessage((char*)UART_rxBuff);
}

void NumToChar(uint32_t number, unsigned char *result, uint16_t* lenght)
{
	unsigned char buffer[10]; //a temp array to hold results of conversion
	uint8_t i=1; //loop index

	buffer[0] = '\0';
	do
	{
		buffer[i++] = (number % 10) + '0'; //convert integer to character
		number /= 10;
		*lenght = *lenght + 1;
	} while(number);

	for(i=0; i<=*lenght; i++){
    	result[*lenght-i] = buffer[i];
    }
}

HAL_StatusTypeDef UART_Send(uint8_t* data, uint8_t size)
{
	printf("%lu: ",++UARTMessageNumber);
	if(HAL_UART_Transmit(&UartHandle, data, size, 0xFFFF) != HAL_OK)
		Error_Handler();
	return HAL_OK;
}

HAL_StatusTypeDef UART_PutC(const char c)
{
	if(UART_Send((uint8_t*)&c, 1) != HAL_OK)
		Error_Handler();
	return HAL_OK;
}

HAL_StatusTypeDef UART_NewLine()
{
	char new_line[2]={"\n\r"};
	if(HAL_UART_Transmit(&UartHandle, (uint8_t*)&new_line, 2, 0x000F) != HAL_OK)
		Error_Handler();
	return HAL_OK;
}

void UART_SendString(const char* string)
{
	printf("%lu: ",++UARTMessageNumber);
	printf("%s \n\r",string);
}

void UART_SendInt(const int number)
{
	printf("%lu: ",++UARTMessageNumber);
	printf("%d \n\r", number);
}

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}
