/*
 * Inerc.h
 *
 *  Created on: 2016. dec. 14.
 *      Author: Muszka
 */

#ifndef BSP_IMU_H_
#define BSP_IMU_H_

#include <BSP.h>
#include "LSM6DS3_ACC_GYRO_driver_HL.h"
#include "Timers.h"

/* Definition for SPI1 clock resources */
#define	SPI_IMU							SPI2
#define SPI_IMU_CLK_ENABLE()           	__HAL_RCC_SPI2_CLK_ENABLE()
#define SPI_IMU_GPIO_CLK_ENABLE()      	__HAL_RCC_GPIOB_CLK_ENABLE()

/* Definition for SPI Pins
 * NSS  - PB12
 * SCLK - PB13
 * MISO - PB14
 * MOSI - PB15 */
#define SPI_IMU_NSS_PIN					GPIO_PIN_12
#define SPI_IMU_SCLK_PIN               	GPIO_PIN_13
#define SPI_IMU_MISO_PIN               	GPIO_PIN_14
#define SPI_IMU_MOSI_PIN               	GPIO_PIN_15
#define SPI_IMU_GPIO_PORT              	GPIOB
#define SPI_IMU_GPIO_AF					GPIO_AF5_SPI2

#define SPI_IMU_IRQn					SPI2_IRQn
#define SPI_IMU_IRQHandler				SPI2_IRQHandler

//Szoftveres chip select
#define IMU_CS_High()		HAL_GPIO_WritePin(SPI_IMU_GPIO_PORT, SPI_IMU_NSS_PIN, GPIO_PIN_SET)
#define IMU_CS_Low()		HAL_GPIO_WritePin(SPI_IMU_GPIO_PORT, SPI_IMU_NSS_PIN, GPIO_PIN_RESET)

/*Global variables*/
extern SPI_HandleTypeDef SpiHandle_IMU;
extern DrvContextTypeDef GYRO_SensorHandle;
extern DrvContextTypeDef ACCELERO_SensorHandle;
extern GYRO_Drv_t* GYRO_Driver;
extern ACCELERO_Drv_t* ACCELERO_Driver;

/* Functions */
void SPI_IMU_Init();
void SPI_IMU_TransmitNonBlocking(SPI_HandleTypeDef* handle, uint8_t* pBuffer, uint16_t length);
void SPI_IMU_TransmitReceiveNonBlocking(SPI_HandleTypeDef* handle, uint8_t* RxBuffer, uint8_t* TxBuffer, uint16_t length);
void SPI_IMU_ReceiveNonBlocking(SPI_HandleTypeDef* handle, uint8_t* pBuffer, uint16_t length);
////ACC_GYRO driver
uint8_t Sensor_IO_Write(void *handle, uint8_t WriteAddr, uint8_t *pBuffer, uint16_t nBytesToWrite);
uint8_t Sensor_IO_Read(void *handle, uint8_t ReadAddr, uint8_t *pBuffer, uint16_t nBytesToRead);

void ACCELERO_SensorHandle_Init();
void GYRO_SensorHandle_Init();

void IMU_Init();

void Calibrate_Gyro();
void Gyro_Callback();
void GetDegree_X(int32_t* degree_x);
void GetDegree_Y(int32_t* degree_y);
void GetDegree_Z(int32_t* degree_z);
void ResetDegrees();
void IMU_Degre_Calc_Start();
void IMU_Degre_Calc_Stop();


#endif /* BSP_IMU_H_ */
