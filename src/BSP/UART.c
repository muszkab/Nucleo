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
UART_HandleTypeDef UartHandle_Cable;
UART_HandleTypeDef UartHandle_Bluetooth;
UART_HandleTypeDef UartHandle_Radio;
/* Uart receive message buffer */
uint8_t UART_rxBuff[1]={'0'};
uint32_t UARTMessageNumber=0;

/**
  * @brief UART Cable USART2 Initialization
  */
void UART_Cable_Init(){
	GPIO_InitTypeDef  GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* Enable GPIO TX/RX clock */
	USART_CABLE_TX_GPIO_CLK_ENABLE();
	USART_CABLE_RX_GPIO_CLK_ENABLE();

	/* Enable USART2 clock */
	USART_CABLE_CLK_ENABLE();

	 /*##-2- Configure peripheral GPIO ##########################################*/
	/* UART TX GPIO pin configuration  */
	GPIO_InitStruct.Pin       = USART_CABLE_TX_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = USART_CABLE_TX_AF;

	HAL_GPIO_Init(USART_CABLE_TX_GPIO_PORT, &GPIO_InitStruct);

	/* UART RX GPIO pin configuration  */
	GPIO_InitStruct.Pin = USART_CABLE_RX_PIN;
	GPIO_InitStruct.Alternate = USART_CABLE_RX_AF;

	HAL_GPIO_Init(USART_CABLE_RX_GPIO_PORT, &GPIO_InitStruct);

	/* Peripheral interrupt init */
	HAL_NVIC_SetPriority(USART_CABLE_IRQn,1,0);
	HAL_NVIC_EnableIRQ(USART_CABLE_IRQn);

	/*##-3- Configure the UART peripheral ######################################*/
	/* Put the USART peripheral in the Asynchronous mode (UART Mode) */
	/* UART1 configured as follows:
	  - Word Length = 8 Bits
	  - Stop Bit = One Stop bit
	  - Parity = No parity
	  - BaudRate = 9600 baud
	  - Hardware flow control disabled (RTS and CTS signals) */
	UartHandle_Cable.Instance          = USART_CABLE;
	UartHandle_Cable.Init.BaudRate     = 9600;
	UartHandle_Cable.Init.WordLength   = UART_WORDLENGTH_8B;
	UartHandle_Cable.Init.StopBits     = UART_STOPBITS_1;
	UartHandle_Cable.Init.Parity       = UART_PARITY_NONE;
	UartHandle_Cable.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	UartHandle_Cable.Init.Mode         = UART_MODE_TX_RX;
	UartHandle_Cable.Init.OverSampling = UART_OVERSAMPLING_16;

	if(HAL_UART_Init(&UartHandle_Cable) != HAL_OK){
		/* Initialization Error */
		Error_Handler();
	}
}

/**
  * @brief UART Bluetooth USART1 Initialization
  */
void UART_Bluetooth_Init(){
	GPIO_InitTypeDef  GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* Enable GPIO TX/RX clock */
	USART_BlUETOOTH_RX_GPIO_CLK_ENABLE();
	USART_BlUETOOTH_TX_GPIO_CLK_ENABLE();

	/* Enable USART1 clock */
	USART_BlUETOOTH_CLK_ENABLE();

	/*##-2- Configure peripheral GPIO ##########################################*/
	/* UART TX GPIO pin configuration  */
	GPIO_InitStruct.Pin       = USART_BlUETOOTH_TX_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = USART_BlUETOOTH_TX_AF;

	HAL_GPIO_Init(USART_BlUETOOTH_TX_GPIO_PORT, &GPIO_InitStruct);

	/* UART RX GPIO pin configuration  */
	GPIO_InitStruct.Pin = USART_BlUETOOTH_RX_PIN;
	GPIO_InitStruct.Alternate = USART_BlUETOOTH_RX_AF;

	HAL_GPIO_Init(USART_BlUETOOTH_RX_GPIO_PORT, &GPIO_InitStruct);

	/* Peripheral interrupt init */
	HAL_NVIC_SetPriority(USART_BlUETOOTH_IRQn,1,0);
	HAL_NVIC_EnableIRQ(USART_BlUETOOTH_IRQn);

	/*##-3- Configure the UART peripheral ######################################*/
	/* Put the USART peripheral in the Asynchronous mode (UART Mode) */
	/* UART1 configured as follows:
	  - Word Length = 8 Bits
	  - Stop Bit = One Stop bit
	  - Parity = No parity
	  - BaudRate = 115200 baud
	  - Hardware flow control disabled (RTS and CTS signals) */
	UartHandle_Bluetooth.Instance          = USART_BlUETOOTH;
	UartHandle_Bluetooth.Init.BaudRate     = 115200;
	UartHandle_Bluetooth.Init.WordLength   = UART_WORDLENGTH_8B;
	UartHandle_Bluetooth.Init.StopBits     = UART_STOPBITS_1;
	UartHandle_Bluetooth.Init.Parity       = UART_PARITY_NONE;
	UartHandle_Bluetooth.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	UartHandle_Bluetooth.Init.Mode         = UART_MODE_TX_RX;
	UartHandle_Bluetooth.Init.OverSampling = UART_OVERSAMPLING_16;

	if(HAL_UART_Init(&UartHandle_Bluetooth) != HAL_OK){
		/* Initialization Error */
		Error_Handler();
	}
}
/**
  * @brief UART Radio UART5 Initialization
  */
void UART_Radio_Init(){
	GPIO_InitTypeDef  GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* Enable GPIO TX/RX clock */
	USART_RADIO_RX_GPIO_CLK_ENABLE();
	USART_RADIO_TX_GPIO_CLK_ENABLE();

	/* Enable UART5 clock */
	USART_RADIO_CLK_ENABLE();

	/*##-2- Configure peripheral GPIO ##########################################*/
	/* UART TX GPIO pin configuration  */
	GPIO_InitStruct.Pin       = USART_RADIO_TX_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = USART_RADIO_TX_AF;

	HAL_GPIO_Init(USART_RADIO_TX_GPIO_PORT, &GPIO_InitStruct);

	/* UART RX GPIO pin configuration  */
	GPIO_InitStruct.Pin = USART_RADIO_RX_PIN;
	GPIO_InitStruct.Alternate = USART_RADIO_RX_AF;

	HAL_GPIO_Init(USART_RADIO_RX_GPIO_PORT, &GPIO_InitStruct);

	/* Peripheral interrupt init */
	HAL_NVIC_SetPriority(USART_RADIO_IRQn,4,0);
	HAL_NVIC_EnableIRQ(USART_RADIO_IRQn);

	/*##-3- Configure the UART peripheral ######################################*/
	/* Put the USART peripheral in the Asynchronous mode (UART Mode) */
	/* UART5 configured as follows:
	  - Word Length = 8 Bits
	  - Stop Bit = One Stop bit
	  - Parity = No parity
	  - BaudRate = 115200 baud
	  - Hardware flow control disabled (RTS and CTS signals) */
	UartHandle_Radio.Instance          = USART_RADIO;
	UartHandle_Radio.Init.BaudRate     = 115200;
	UartHandle_Radio.Init.WordLength   = UART_WORDLENGTH_8B;
	UartHandle_Radio.Init.StopBits     = UART_STOPBITS_1;
	UartHandle_Radio.Init.Parity       = UART_PARITY_NONE;
	UartHandle_Radio.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	UartHandle_Radio.Init.Mode         = UART_MODE_TX_RX;
	UartHandle_Radio.Init.OverSampling = UART_OVERSAMPLING_16;

	if(HAL_UART_Init(&UartHandle_Radio) != HAL_OK){
		/* Initialization Error */
		Error_Handler();
	}

	//TODO Indítani nem itt kell, buffer át kell állítani
	if(HAL_UART_Receive_IT(&UartHandle_Radio,UART_rxBuff, sizeof(UART_rxBuff)) != HAL_OK){
		Error_Handler();
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if ( huart->Instance == USART_CABLE)	{
		UART_SendStringNonBlocking((char*)UART_rxBuff, huart);
		UART_RecvStringNonBlocking(huart);
	}
}

void UART_SendNumberBlocking(uint32_t number, UART_HandleTypeDef *UartHandle)
{
	unsigned char result[10];
	uint16_t lenght = 0;

	NumToChar(number,result,&lenght);
	//UART1_WaitForReadyToSend();
	if(HAL_UART_Transmit(UartHandle, result,lenght,1000) != HAL_OK){
		Error_Handler();
	}
}
void UART_SendNumberNonBlocking(uint32_t number, UART_HandleTypeDef *UartHandle)
{
	unsigned char result[10];
	uint16_t lenght = 0;

	//UART1_WaitForReadyToSend();
	NumToChar(number,result,&lenght);
	if(HAL_UART_Transmit_IT(UartHandle, result,lenght) != HAL_OK){
		Error_Handler();
	}
}
void UART_SendStringBlocking(char* string, UART_HandleTypeDef *UartHandle)
{
	uint16_t len = 0;
	char* lstring = string;
	while (*lstring)
	{
		len++;
		lstring++;
	}
	//UART_WaitForReadyToSend(UartHandle);
	if(HAL_UART_Transmit(UartHandle,(uint8_t*) string,len,1000) != HAL_OK){
		Error_Handler();
	}
}
void UART_SendStringNonBlocking(char* string, UART_HandleTypeDef *UartHandle)
{
	uint16_t len = 0;
	char* lstring = string;
	while (*lstring)
	{
		len++;
		lstring++;
	}
	//UART1_WaitForReadyToSend();
	if(HAL_UART_Transmit_IT(UartHandle,(uint8_t*) string,len) != HAL_OK){
		Error_Handler();
	}
}

void UART_WaitForReadyToSend(UART_HandleTypeDef *UartHandle)
{
	//while ((UartHandle.State != HAL_UART_STATE_BUSY_RX)
	//		&& (UartHandle.State != HAL_UART_STATE_READY));
	while (HAL_UART_GetState(UartHandle) != HAL_UART_STATE_READY){}
}

void UART_RecvStringNonBlocking(UART_HandleTypeDef *UartHandle)
{
	if(HAL_UART_Receive_IT(UartHandle, UART_rxBuff, sizeof(UART_rxBuff)) != HAL_OK){
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
	if(HAL_UART_Transmit(&UartHandle_Cable, data, size, 0xFFFF) != HAL_OK)
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
	if(HAL_UART_Transmit(&UartHandle_Cable, (uint8_t*)&new_line, 2, 0x000F) != HAL_OK)
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
  HAL_UART_Transmit(&UartHandle_Cable, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}
