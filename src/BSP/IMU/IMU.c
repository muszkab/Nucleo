/*
 * Inerc.c
 *
 *  Created on: 2016. dec. 14.
 *      Author: Muszka
 */

#include "IMU.h"
#include "Leds.h"

/*SPI1 handler declaration */
SPI_HandleTypeDef SpiHandle_IMU;

void SPI_IMU_Init()
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* Enable GPIO TX clock */
	SPI_IMU_GPIO_CLK_ENABLE();
	/* Enable SPI clock */
	SPI_IMU_CLK_ENABLE();

	/* SPI SCLK GPIO pin configuration  */
	GPIO_InitStruct.Pin       = SPI_IMU_SCLK_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = SPI_IMU_GPIO_AF;
	HAL_GPIO_Init(SPI_IMU_GPIO_PORT, &GPIO_InitStruct);

	/* SPI MOSI GPIO pin configuration */
	GPIO_InitStruct.Pin = SPI_IMU_MOSI_PIN;
	HAL_GPIO_Init(SPI_IMU_GPIO_PORT, &GPIO_InitStruct);

	/* SPI MISO GPIO pin configuration */
	GPIO_InitStruct.Pin = SPI_IMU_MISO_PIN;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	HAL_GPIO_Init(SPI_IMU_GPIO_PORT, &GPIO_InitStruct);

	/* SPI NSS GPIO pin configuration */
	GPIO_InitStruct.Pin = SPI_IMU_MISO_PIN;
	HAL_GPIO_Init(SPI_IMU_GPIO_PORT, &GPIO_InitStruct);

	/*##-3- Configure the NVIC for SPI #########################################*/
	/* NVIC for SPI */
	HAL_NVIC_SetPriority(SPI_IMU_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(SPI_IMU_IRQn);

	/* Set the SPI parameters */
	SpiHandle_IMU.Instance = SPI_IMU;
	SpiHandle_IMU.Init.Mode = SPI_MODE_MASTER;
	SpiHandle_IMU.Init.Direction = SPI_DIRECTION_2LINES;
	SpiHandle_IMU.Init.DataSize = SPI_DATASIZE_8BIT;
	SpiHandle_IMU.Init.CLKPolarity = SPI_POLARITY_HIGH;
	SpiHandle_IMU.Init.CLKPhase = SPI_PHASE_2EDGE;
	SpiHandle_IMU.Init.NSS = SPI_NSS_HARD_OUTPUT;
	SpiHandle_IMU.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	SpiHandle_IMU.Init.FirstBit = SPI_FIRSTBIT_MSB;
	SpiHandle_IMU.Init.TIMode = SPI_TIMODE_DISABLE;
	SpiHandle_IMU.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	SpiHandle_IMU.Init.CRCPolynomial = 10;

	if (HAL_SPI_Init(&SpiHandle_IMU) != HAL_OK){
		Error_SendUart("SPI_IMU init error. \n\r");
	}
}

//csak k�ld�s
void SPI_IMU_TransmitNonBlocking(SPI_HandleTypeDef* handle, uint8_t* pBuffer, uint16_t length)
{
	if(HAL_SPI_Transmit_IT(handle, pBuffer, length) != HAL_OK)
	{
		Error_SendUart("SPI IMU nonblocking transmit error. \n\r");
	}
}

//k�ld�s �s fogad�s
void SPI_IMU_TransmitReceiveNonBlocking(SPI_HandleTypeDef* handle, uint8_t* TxBuffer, uint8_t* RxBuffer, uint16_t length)
{
	//length: k�ld�tt �s fogadott byte-ok darabsz�ma �sszesen??
	if(HAL_SPI_TransmitReceive_IT(handle, TxBuffer, RxBuffer, length) != HAL_OK)
	{
		Error_SendUart("SPI IMU nonblocking transmit error. \n\r");
	}
}


//ACC_GYRO driverhez
uint8_t Sensor_IO_Write(void *handle, uint8_t WriteAddr, uint8_t *pBuffer, uint16_t nBytesToWrite)
{
	//if(handle == &SpiHandle_IMU)
	//{
		//SPI adatcsomag �ssze�ll�t�s
		uint8_t data[nBytesToWrite+1];
		//els� byte a c�m, �r�s: MSB=0
		data[0] = WriteAddr + 0;
		//adat �tm�sol�s
		for(int i=0;i<nBytesToWrite;i++)
			data[i+1] = pBuffer[i];

		//k�ld�s, a hossz eggyel nagyobb a c�m miatt
		SPI_IMU_TransmitNonBlocking(&SpiHandle_IMU, data, (nBytesToWrite+1));
		//ACC_GYRO driver miatt
		return 0;
	//}
	//ACC_GYRO driver miatt
	//return 1;
}

//ACC_GYRO driverhez
uint8_t Sensor_IO_Read(void *handle, uint8_t ReadAddr, uint8_t *pBuffer, uint16_t nBytesToRead)
{
	//if(handle == &SpiHandle_IMU)
	//{
		//k�ld�nk: 8 bites c�m
		uint8_t TxBuffer[1];
		//els� byte a c�m, olvas�s: MSB=1
		TxBuffer[0] = ReadAddr + (1<<7); //(10000000b)

		SPI_IMU_TransmitReceiveNonBlocking(&SpiHandle_IMU, TxBuffer, pBuffer, (nBytesToRead));

		//ACC_GYRO driver miatt
		return 0;
	//}
	//ACC_GYRO driver miatt
	//return 1;
}


void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if(hspi->Instance == SPI_IMU)
		LED_Toggle(Red);
}


void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
	Error_SendUart("SPI return callback error. \n\r");
}
