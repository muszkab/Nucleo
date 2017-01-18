/*
 * Inerc.c
 *
 *  Created on: 2016. dec. 14.
 *      Author: Muszka
 */

#include "IMU.h"
#include "Timers.h"


/**				    EL�RE
 *            /\    /\
 *            z     /
 *            |    y
 *            |   /
 *   JOBB     |  /    BAL
 *            | /
 *            |/
 * <x----------
 * */

/*SPI1 handler declaration */
SPI_HandleTypeDef SpiHandle_IMU;

/*  */
DrvContextTypeDef GYRO_SensorHandle;
DrvContextTypeDef ACCELERO_SensorHandle;
GYRO_Drv_t* 	GYRO_Driver;
ACCELERO_Drv_t* ACCELERO_Driver;

/* GYRO*/
#define TIME_CYCLE 			((float)GYRO_TIM_PERIOD/(float)GYRO_TIM_FREQ)
#define CAL_VALUE 			1000
uint8_t GYRO_Who=0;
static SensorAxes_t GYRO_AXIS;
static SensorAxes_t GYRO_AXIS_CAL;  //value to calibration
static int32_t Degrees[3];	 //X, Y, Z tengely k�r�li elfordul�s

/* Id�k�z�nk�nt sz�g�m�t�s elv�gz�se */
void Gyro_Callback(){
	GYRO_Driver->Get_Axes(&GYRO_SensorHandle,&GYRO_AXIS);
	GYRO_AXIS.AXIS_X -= GYRO_AXIS_CAL.AXIS_X;
	GYRO_AXIS.AXIS_Y -= GYRO_AXIS_CAL.AXIS_Y;
	GYRO_AXIS.AXIS_Z -= GYRO_AXIS_CAL.AXIS_Z;

	Degrees[0] += GYRO_AXIS.AXIS_X * TIME_CYCLE;
	Degrees[1] += GYRO_AXIS.AXIS_Y * TIME_CYCLE;
	Degrees[2] += GYRO_AXIS.AXIS_Z * TIME_CYCLE;
}

void IMU_Degre_Calc_Start(){
	/*##-1- Start timer #######################################*/
	if (HAL_TIM_Base_Start_IT(&TimHandle_Gyro) != HAL_OK)	{
		Error_Handler();
	}
}

void IMU_Degre_Calc_Stop(){
	/*##-1- Stop timer #######################################*/
	if (HAL_TIM_Base_Stop_IT(&TimHandle_Gyro) != HAL_OK)	{
		Error_Handler();
	}
}

/* Sz�ginform�ci�k lek�rdez�se
 * X,Y,Z sorrendben*/
int32_t GetDegree_X(){
	return Degrees[0];
}

int32_t GetDegree_Y(){
	return Degrees[1];
}

int32_t GetDegree_Z(){
	return Degrees[2];
}

void ResetDegrees(){
	Degrees[0] = 0;
	Degrees[1] = 0;
	Degrees[2] = 0;
}


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



//csak k�ld�s
void SPI_IMU_TransmitBlocking(SPI_HandleTypeDef* handle, uint8_t* pBuffer, uint16_t length)
{
	if(HAL_SPI_Transmit(handle, pBuffer, length, 20) != HAL_OK)
	{
		Error_SendUart("SPI IMU nonblocking transmit error. \n\r");
	}
}

//csak fogad�s
void SPI_IMU_ReceiveBlocking(SPI_HandleTypeDef* handle, uint8_t* pBuffer, uint16_t length)
{
	if(HAL_SPI_Receive(handle, pBuffer, length, 20) != HAL_OK)
	{
		Error_SendUart("SPI IMU nonblocking receive error. \n\r");
	}
}

//k�ld�s �s fogad�s
void SPI_IMU_TransmitReceiveBlocking(SPI_HandleTypeDef* handle, uint8_t* TxBuffer, uint8_t* RxBuffer, uint16_t length)
{
	//length: k�ld�tt �s fogadott byte-ok darabsz�ma fejenk�nt
	if(HAL_SPI_TransmitReceive(handle, TxBuffer, RxBuffer, length, 20) != HAL_OK)
	{
		Error_SendUart("SPI IMU nonblocking transmit-receive error. \n\r");
	}
}


//ACC_GYRO driverhez
uint8_t Sensor_IO_Write(void *handle, uint8_t WriteAddr, uint8_t *pBuffer, uint16_t nBytesToWrite)
{
	//SPI adatcsomag �ssze�ll�t�s
	uint8_t data[nBytesToWrite+1];
	//els� byte a c�m, �r�s: MSB=0
	data[0] = WriteAddr + 0;
	//adat �tm�sol�s
	for(int i=0;i<nBytesToWrite;i++)
		data[i+1] = pBuffer[i];

	//szoftver chip select
	IMU_CS_Low();
	//k�ld�s, a hossz eggyel nagyobb a c�m miatt
	SPI_IMU_TransmitBlocking(&SpiHandle_IMU, data, (nBytesToWrite+1));

	IMU_CS_High();

	//ACC_GYRO driver miatt
	return 0;
}

//ACC_GYRO driverhez
uint8_t Sensor_IO_Read(void *handle, uint8_t ReadAddr, uint8_t *pBuffer, uint16_t nBytesToRead)
{
	//k�ld�nk: 8 bites c�m
	uint8_t Address;
	uint8_t DUMMY=0;
	//els� byte a c�m, olvas�s: MSB=1
	Address = ReadAddr + (1<<7); //(10000000b)

	//szoftver chip select
	IMU_CS_Low();

	//c�m k�ld�se
	SPI_IMU_TransmitBlocking(&SpiHandle_IMU, &Address, 1);
	//adat fogad�sa
	SPI_IMU_TransmitReceiveBlocking(&SpiHandle_IMU, &DUMMY, pBuffer, nBytesToRead);

	IMU_CS_High();

	//ACC_GYRO driver miatt
	return 0;
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
	GYRO_TIM_Init();

	/* IMU */
	GYRO_Driver->Get_WhoAmI(&GYRO_SensorHandle,&GYRO_Who);
	if( ACCELERO_Driver->Init(&ACCELERO_SensorHandle) != COMPONENT_OK )
		Error_SendUart("ACC Init error!");
	if( ACCELERO_Driver->Sensor_Enable(&ACCELERO_SensorHandle) != COMPONENT_OK )
		Error_SendUart("ACC Enable error!");
	if( GYRO_Driver->Init(&GYRO_SensorHandle) != COMPONENT_OK )
		Error_SendUart("GYRO Init error!");
	if( GYRO_Driver->Sensor_Enable(&GYRO_SensorHandle) != COMPONENT_OK )
		Error_SendUart("GYRO Enable error!");
	GYRO_Driver->Set_FS(&GYRO_SensorHandle,FS_MID);
}

/* Az offszet kiszed�se sok minta �tlag�val */
void Calibrate_Gyro(){
	uint8_t status;
	IMU_Degre_Calc_Stop();
	GYRO_AXIS_CAL.AXIS_X = 0;
	GYRO_AXIS_CAL.AXIS_Y = 0;
	GYRO_AXIS_CAL.AXIS_Z = 0;
	for(int i = 0; i<CAL_VALUE;i++){
		while(status == LSM6DS3_ACC_GYRO_GDA_NO_DATA_AVAIL){
			if(GYRO_Driver->Get_DRDY_Status(&GYRO_SensorHandle,&status ) != COMPONENT_OK)
				Error_SendUart("IMU calibration error. \n\r");
		}
		GYRO_Driver->Get_Axes(&GYRO_SensorHandle,&GYRO_AXIS);
		GYRO_AXIS_CAL.AXIS_X += GYRO_AXIS.AXIS_X;
		GYRO_AXIS_CAL.AXIS_Y += GYRO_AXIS.AXIS_Y;
		GYRO_AXIS_CAL.AXIS_Z += GYRO_AXIS.AXIS_Z;
		status = 0;
	}

	GYRO_AXIS_CAL.AXIS_X = GYRO_AXIS_CAL.AXIS_X/CAL_VALUE;
	GYRO_AXIS_CAL.AXIS_Y = GYRO_AXIS_CAL.AXIS_Y/CAL_VALUE;
	GYRO_AXIS_CAL.AXIS_Z = GYRO_AXIS_CAL.AXIS_Z/CAL_VALUE;
}
