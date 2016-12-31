/*
 * Encoder.c
 *
 *  Created on: 2016. dec. 23.
 *      Author: szabgyul92
 */

/* Includes ------------------------------------------------------------------*/
#include "Encoder.h"

/* Timer handler declaration */
TIM_HandleTypeDef    Encoder_Handle;
/* Timer Encoder Configuration Structure declaration */
TIM_Encoder_InitTypeDef sEncoderConfig;

/**
  * @brief TIM MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim){
	GPIO_InitTypeDef   GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* TIM1 Peripheral clock enable */
	ENC_TIM_CLK_ENABLE();

	/* Enable GPIO Channels Clock */
	ENC_GPIO_CLK_ENABLE();

	/*##-2- Configure I/Os #####################################################*/
	/* Common configuration for all channels */
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;

	/* Channel 1 and Channel 2 configuration */
	GPIO_InitStruct.Pin = ENC_TIM_GPIO_PIN_CHANNEL1 | ENC_TIM_GPIO_PIN_CHANNEL2;
	HAL_GPIO_Init(ENC_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief Encoder Initialization
  */
void Encoder_Init(){
	/* -1- Initialize TIM2 to handle the encoder sensor */
	/* Initialize TIM2 peripheral as follows:
	   + Period = 4294967295
	   + Prescaler = 0
	   + ClockDivision = 0
	   + Counter direction = Up
	*/
	Encoder_Handle.Instance = ENC_TIM;

	Encoder_Handle.Init.Period            = 4294967295;
	Encoder_Handle.Init.Prescaler         = 0;
	Encoder_Handle.Init.ClockDivision     = 0;
	Encoder_Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
	Encoder_Handle.Init.RepetitionCounter = 0;

	sEncoderConfig.EncoderMode        = TIM_ENCODERMODE_TI12;

	sEncoderConfig.IC1Polarity        = TIM_ICPOLARITY_RISING;
	sEncoderConfig.IC1Selection       = TIM_ICSELECTION_DIRECTTI;
	sEncoderConfig.IC1Prescaler       = TIM_ICPSC_DIV1;
	sEncoderConfig.IC1Filter          = 0;

	sEncoderConfig.IC2Polarity        = TIM_ICPOLARITY_RISING;
	sEncoderConfig.IC2Selection       = TIM_ICSELECTION_DIRECTTI;
	sEncoderConfig.IC2Prescaler       = TIM_ICPSC_DIV1;
	sEncoderConfig.IC2Filter          = 0;

	if(HAL_TIM_Encoder_Init(&Encoder_Handle, &sEncoderConfig) != HAL_OK){
		/* Initialization Error */
		Error_Handler();
	}

	/* Start the encoder interface */
	/* TODO az indítást máshová is lehet rakni */
	HAL_TIM_Encoder_Start(&Encoder_Handle, TIM_CHANNEL_ALL);
}
