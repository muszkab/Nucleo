/*
 * Inerc.c
 *
 *  Created on: 2016. dec. 14.
 *      Author: Muszka
 */

#include "IMU.h"

typedef enum{
	Write,
	Read
}IOState;
//segédváltozó, hogy az IMU_CS_High() ne lépjen érvénybe túl hamar
static IOState State;

/*SPI1 handler declaration */
SPI_HandleTypeDef SpiHandle_IMU;

/*  */
DrvContextTypeDef GYRO_SensorHandle;
DrvContextTypeDef ACCELERO_SensorHandle;
GYRO_Drv_t* 	GYRO_Driver;
ACCELERO_Drv_t* ACCELERO_Driver;

void SPI_IMU_Init()
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* Enable GPIO TX clock */
	SPI_IMU_GPIO_CLK_ENABLE();
	/* Enable SPI clock */
	SPI_IMU_CLK_ENABLE();

	/* SPI SCLK, MOSI, MISO GPIO pin configuration  */
	GPIO_InitStruct.Pin       = SPI_IMU_SCLK_PIN | SPI_IMU_MOSI_PIN | SPI_IMU_MISO_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed     = GPIO_SPEED_MEDIUM;
	GPIO_InitStruct.Alternate = SPI_IMU_GPIO_AF;
	HAL_GPIO_Init(SPI_IMU_GPIO_PORT, &GPIO_InitStruct);

	/* SPI NSS GPIO pin configuration */
	GPIO_InitStruct.Pin 	  = SPI_IMU_NSS_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_MEDIUM;
	//GPIO_InitStruct.Alternate = SPI_IMU_GPIO_AF;
	HAL_GPIO_Init(SPI_IMU_GPIO_PORT, &GPIO_InitStruct);
	IMU_CS_High();

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
	SpiHandle_IMU.Init.NSS = SPI_NSS_SOFT;
	SpiHandle_IMU.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	SpiHandle_IMU.Init.FirstBit = SPI_FIRSTBIT_MSB;
	SpiHandle_IMU.Init.TIMode = SPI_TIMODE_DISABLE;
	SpiHandle_IMU.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	SpiHandle_IMU.Init.CRCPolynomial = 7;

	if (HAL_SPI_Init(&SpiHandle_IMU) != HAL_OK){
		Error_SendUart("SPI_IMU init error. \n\r");
	}
}



//csak küldés
void SPI_IMU_TransmitNonBlocking(SPI_HandleTypeDef* handle, uint8_t* pBuffer, uint16_t length)
{
	if(HAL_SPI_Transmit(handle, pBuffer, length, 10) != HAL_OK)
	{
		Error_SendUart("SPI IMU nonblocking transmit error. \n\r");
	}
}

//csak fogadás
void SPI_IMU_ReceiveNonBlocking(SPI_HandleTypeDef* handle, uint8_t* pBuffer, uint16_t length)
{
	if(HAL_SPI_Receive(handle, pBuffer, length, 10) != HAL_OK)
	{
		Error_SendUart("SPI IMU nonblocking receive error. \n\r");
	}
}

//küldés és fogadás
void SPI_IMU_TransmitReceiveNonBlocking(SPI_HandleTypeDef* handle, uint8_t* TxBuffer, uint8_t* RxBuffer, uint16_t length)
{
	//length: küldött és fogadott byte-ok darabszáma fejenként
	if(HAL_SPI_TransmitReceive(handle, TxBuffer, RxBuffer, length, 10) != HAL_OK)
	{
		Error_SendUart("SPI IMU nonblocking transmit-receive error. \n\r");
	}
}


//ACC_GYRO driverhez
uint8_t Sensor_IO_Write(void *handle, uint8_t WriteAddr, uint8_t *pBuffer, uint16_t nBytesToWrite)
{
	//SPI adatcsomag összeállítás
	uint8_t data[nBytesToWrite+1];
	//elsõ byte a cím, írás: MSB=0
	data[0] = WriteAddr + 0;
	//adat átmásolás
	for(int i=0;i<nBytesToWrite;i++)
		data[i+1] = pBuffer[i];

	//szoftver chip select
	IMU_CS_Low();
	//TxCpltCallback-ben fusson le a IMU_CS_High(), ha a state==write, több kommunikáció már nem lesz
	State = Write;
	//küldés, a hossz eggyel nagyobb a cím miatt
	SPI_IMU_TransmitNonBlocking(&SpiHandle_IMU, data, (nBytesToWrite+1));
	//callback függvényben van az IMU_CS_High()
	IMU_CS_High();

	//ACC_GYRO driver miatt
	return 0;
}

//ACC_GYRO driverhez
uint8_t Sensor_IO_Read(void *handle, uint8_t ReadAddr, uint8_t *pBuffer, uint16_t nBytesToRead)
{
	//küldünk: 8 bites cím
	uint8_t Address;
	uint8_t DUMMY=0;
	//elsõ byte a cím, olvasás: MSB=1
	Address = ReadAddr + (1<<7); //(10000000b)

	//szoftver chip select
	IMU_CS_Low();
	//IMU_CS_High() jó helyen fusson le a callbackben, ne a sima transmit után
	State = Read;
	//cím küldése
	SPI_IMU_TransmitNonBlocking(&SpiHandle_IMU, &Address, 1);
	//adat fogadása
	SPI_IMU_TransmitReceiveNonBlocking(&SpiHandle_IMU, &DUMMY, pBuffer, nBytesToRead);
	//callback függvényben van az IMU_CS_High()
	IMU_CS_High();

	//ACC_GYRO driver miatt
	return 0;
}

//TODO: ha nincs IT, a callback-eket törölni! (IOState State is)
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if(hspi->Instance == SPI_IMU)
	{
		//ha írás volt az spi-n, akkor csak egy transmit fgv fut le, olvasáskor csak a TxRxCpltCallback-ben kell a CS_High()
		if(State == Write)
			IMU_CS_High();
	}
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if(hspi->Instance == SPI_IMU)
		IMU_CS_High();
}


void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
	Error_SendUart("SPI return callback error. \n\r");
}

void GYRO_SensorHandle_Init()
{
	static GYRO_Data_t GYRO_Data;
	static LSM6DS3_G_Data_t LSM6DS3_G_0_Data;

	GYRO_SensorHandle.who_am_i      = LSM6DS3_ACC_GYRO_WHO_AM_I;
	GYRO_SensorHandle.address       = LSM6DS3_ACC_GYRO_I2C_ADDRESS_LOW;
	GYRO_SensorHandle.instance      = 1;	//LSM6DS3_G_0;
	GYRO_SensorHandle.isInitialized = 0;
	GYRO_SensorHandle.isEnabled     = 0;
	GYRO_SensorHandle.isCombo       = 1;
	GYRO_SensorHandle.pData         = ( void * )&GYRO_Data;
	GYRO_SensorHandle.pVTable       = ( void * )&LSM6DS3_G_Drv;
	GYRO_SensorHandle.pExtVTable    = ( void * )&LSM6DS3_G_ExtDrv;

	GYRO_Data.pComponentData = ( void * )&LSM6DS3_G_0_Data;
	GYRO_Data.pExtData       = 0;

	GYRO_Driver = ( GYRO_Drv_t * ) GYRO_SensorHandle.pVTable;
}

void ACCELERO_SensorHandle_Init()
{
	static ACCELERO_Data_t ACCELERO_Data;
	static LSM6DS3_X_Data_t LSM6DS3_X_0_Data;

	ACCELERO_SensorHandle.who_am_i      = LSM6DS3_ACC_GYRO_WHO_AM_I;
	ACCELERO_SensorHandle.address       = LSM6DS3_ACC_GYRO_I2C_ADDRESS_LOW;
	ACCELERO_SensorHandle.instance      = 2;
	ACCELERO_SensorHandle.isInitialized = 0;
	ACCELERO_SensorHandle.isEnabled     = 0;
	ACCELERO_SensorHandle.isCombo       = 1;
	ACCELERO_SensorHandle.pData         = ( void * )&ACCELERO_Data;
	ACCELERO_SensorHandle.pVTable       = ( void * )&LSM6DS3_X_Drv;
	ACCELERO_SensorHandle.pExtVTable    = ( void * )&LSM6DS3_X_ExtDrv;

	ACCELERO_Data.pComponentData = ( void * )&LSM6DS3_X_0_Data;
	ACCELERO_Data.pExtData       = 0;

	ACCELERO_Driver = ( ACCELERO_Drv_t * ) ACCELERO_SensorHandle.pVTable;
}

void IMU_Init()
{
	SPI_IMU_Init();
	ACCELERO_SensorHandle_Init();
	GYRO_SensorHandle_Init();
}
