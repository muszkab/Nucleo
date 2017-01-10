/*
 * Sharp.c
 *
 *  Created on: 2017. jan. 10.
 *      Author: szabgyul92
 */

/* Includes ------------------------------------------------------------------*/
#include "Sharp.h"

#define BATT_CONST (float)((0.292*256)/3.3) // (<ellenállás osztó érték> * <ADC felbontás>)/3.3V

#define BufferSize 4
uint16_t uhADC1ConvertedValues[BufferSize];

/* Moving Average */
static const float Weight = 0.8;
static float BatteryV_MAvg;
static float FrontSharp_MAvg;
static float RightSharp_MAvg;
static float LeftSharp_MAvg;
/**
  * @brief Initialize and Start all Sharp sensor and Battery Voltage measurement
  * Read the analog values in every 10 ms
  */
void SharpInit(){
	ADC1_Init();
	ADC1_TIM_Init();

	/*## Start the conversion process #######################################*/
	if(HAL_ADC_Start_DMA(&Adc1Handle, (uint32_t*)&uhADC1ConvertedValues, BufferSize) != HAL_OK){
		/* Start Conversation Error */
		Error_Handler();
	}

	/*## TIM counter enable ################################################*/
	if (HAL_TIM_Base_Start(&Adc1_Tim_Handle) != HAL_OK){
		/* Counter Enable Error */
		Error_Handler();
	}
}

/**
  * @brief  Conversion complete callback in non blocking mode
  * @param  AdcHandle : AdcHandle handle
  * @note   This example shows a simple way to report end of conversion, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle){
	LED_Toggle(Yellow);
	BatteryV_MAvg = ExponentialMovingAverage(uhADC1ConvertedValues[0],
			BatteryV_MAvg, Weight);
	FrontSharp_MAvg = ExponentialMovingAverage(uhADC1ConvertedValues[1],
			FrontSharp_MAvg, Weight);
	RightSharp_MAvg = ExponentialMovingAverage(uhADC1ConvertedValues[3],
			RightSharp_MAvg, Weight);
	LeftSharp_MAvg = ExponentialMovingAverage(uhADC1ConvertedValues[2],
			LeftSharp_MAvg, Weight);
}

/*
 * Exponenciális mozgóátlag
 * NewData: új érték
 * CurrentAvg: eddig kiszámolt mozgóátlag
 * Weight: súly: 0..1
 */
float ExponentialMovingAverage(float NewData, float CurrentAvg, float Weight){
	return Weight*NewData+(1-Weight)*CurrentAvg;
}

float MovingAverage(float NewData, float *MovAvBuff, uint8_t lenght){
	int i;
	float MovAv = 0;
	for(i = 0; i < lenght - 1; i++){
		MovAvBuff[i] = MovAvBuff[i+1];
		MovAv += MovAvBuff[i];
	}
	MovAvBuff[lenght - 1] = NewData;
	MovAv += NewData;
	return MovAv/lenght;
}

float GetBatteryVoltage(){
	return BatteryV_MAvg/BATT_CONST;
}

float GetFrontSharpDistance(){
	return 7026*pow(FrontSharp_MAvg,-1.089);
}
float GetRightSharpDistance(){
	return -9.174*log(0.003454*RightSharp_MAvg);
}
float GetLeftSharpDistance(){
	return 10.3*log(275.08/LeftSharp_MAvg);
}

