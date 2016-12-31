/*
 * stm32f4xx_hal_msp.h
 *
 *  Created on: 2016. szept. 25.
 *      Author: Muszka
 */

#ifndef STM32F4XX_HAL_MSP_H_
#define STM32F4XX_HAL_MSP_H_

//TODO majd logikusabb sorrendben kéne meghívni
#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h" //TODO Ebben mi van, mi lesz? Neve alapján nem lehet behatárolni
#include "Encoder.h"
#include "ADC.h"
#include "UART.h"
#include "Buttons.h"
#include "CAN.h"
#include "Servo.h"
#include "Motor.h"
#include "RemoteControl.h"
#include "Timers.h"
#include "PositionControl.h"
#include "SpeedControl.h"
#include "Leds.h"
#include "LineType.h"
#include "IMU.h"
#include "LSM6DS3_ACC_GYRO_driver_HL.h"
#include "Message.h"

void Periph_Init();
void HAL_MspInit();
void SystemClock_Config(void);

#endif /* STM32F4XX_HAL_MSP_H_ */
