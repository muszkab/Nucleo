/*
 * Sharp.c
 *
 *  Created on: 2017. jan. 10.
 *      Author: szabgyul92
 */

/* Includes ------------------------------------------------------------------*/
#include "Sharp.h"
#include "ADC.h"
#include "math.h"

#define BATT_CONST (float)((0.292*256)/3.3) // (<ellenállás osztó érték> * <ADC felbontás>)/3.3V

#define BufferSize 4
uint16_t uhADC1ConvertedValues[BufferSize];

/* Exponential Moving Average */
/*static const float Weight = 0.8;
static float BatteryV_EMAvg;
static float FrontSharp_EMAvg;
static float RightSharp_EMAvg;
static float LeftSharp_EMAvg; */

/* Moving Average */
#define MABuffLenght 2
static float BatteryV_MAvgBuff[MABuffLenght];
static float FrontSharp_MAvgBuff[MABuffLenght];
static float RightSharp_MAvgBuff[MABuffLenght];
static float LeftSharp_MAvgBuff[MABuffLenght];
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
	/* Exponenciális átlagolás */
	/* BatteryV_EMAvg = ExponentialMovingAverage(uhADC1ConvertedValues[0],
			BatteryV_EMAvg, Weight);
	FrontSharp_EMAvg = ExponentialMovingAverage(uhADC1ConvertedValues[1],
			FrontSharp_EMAvg, Weight);
	RightSharp_EMAvg = ExponentialMovingAverage(uhADC1ConvertedValues[3],
			RightSharp_EMAvg, Weight);
	LeftSharp_EMAvg = ExponentialMovingAverage(uhADC1ConvertedValues[2],
			LeftSharp_EMAvg, Weight); */

	/* Mozgó átlagolás */
	BatteryV_MAvg = MovingAverage((float)uhADC1ConvertedValues[0],
			BatteryV_MAvgBuff, MABuffLenght);
	FrontSharp_MAvg = MovingAverage((float)uhADC1ConvertedValues[1],
			FrontSharp_MAvgBuff, MABuffLenght);
	RightSharp_MAvg = MovingAverage((float)uhADC1ConvertedValues[3],
			RightSharp_MAvgBuff, MABuffLenght);
	LeftSharp_MAvg = MovingAverage((float)uhADC1ConvertedValues[2],
			LeftSharp_MAvgBuff, MABuffLenght);
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
	return (BatteryV_MAvg/BATT_CONST);
}

float GetFrontSharpDistance(){
	return 7026*pow(FrontSharp_MAvg,-1.089);
}

float GetRightSharpDistance(){
	return 849.56*pow(RightSharp_MAvg,-1.001);
}

float GetLeftSharpDistance(){
	return 1069.01*pow(LeftSharp_MAvg,-1.045);
}
