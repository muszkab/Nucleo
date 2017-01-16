/*
 * Sharp.h
 *
 *  Created on: 2017. jan. 10.
 *      Author: szabgyul92
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BSP_SHARP_H_
#define BSP_SHARP_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_msp.h"
#include "math.h"

/*Functions*/
void SharpInit();
float ExponentialMovingAverage(float NewData, float CurrentAvg, float Weight);
float MovingAverage(float NewData, float *MovAvBuff, uint8_t lenght);
float GetBatteryVoltage();
float GetFrontSharpDistance();
float GetRightSharpDistance();
float GetLeftSharpDistance();

#endif /* BSP_SHARP_H_ */
