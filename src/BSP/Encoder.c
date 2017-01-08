/*
 * Encoder.c
 *
 *  Created on: 2016. dec. 23.
 *      Author: szabgyul92
 */

/* Includes ------------------------------------------------------------------*/
#include "Encoder.h"


#define METER_PER_INCR 					1.3459e-4f    /* m/inkrement */
#define ENC_TIME_STEP 					0.05f
#define ENC_FREQ						50

#define Encoder_Callback_Timer_Freq  	40000
#define Encoder_Callback_Timer_Period 	2000

/* Encoder Timer handler declaration */
TIM_HandleTypeDef    	Encoder_Handle;
/* Timer Encoder Configuration Structure declaration */
TIM_Encoder_InitTypeDef sEncoderConfig;
/* Encoder Callback Timer handler declaration */
TIM_HandleTypeDef    	Encoder_Callback_Handle;


static float Velocity = 0;
static float Distance = 0;
static int32_t PrevCnt = 0;
static int32_t CurrCnt = 0;

/**
  * @brief Encoder Initialization
  * 	- Configure the Encoder handle and callback Timers
  * 	- Start the Timers
  */
void Encoder_Init(){
	Encoder_Timer_Init();
	Encoder_Callback_Timer_Init();
	/* Start the encoder interface */
	if(HAL_TIM_Encoder_Start(&Encoder_Handle, TIM_CHANNEL_ALL) != HAL_OK){
		/* Starting Error */
		Error_Handler();
	}
	/* Start the periodical check */
	if (HAL_TIM_Base_Start_IT(&Encoder_Callback_Handle) != HAL_OK){
		/* Starting Error */
		Error_Handler();
	}
}
/**
  * @brief Encoder Callback Timer Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - Handle Configuration
  *           - IT Configuratiob
  */
void Encoder_Callback_Timer_Init(){
	/* Prescaler declaration */
	uint32_t uwPrescalerValue = 0;

	/*##-1- Enable peripheral clock #################################*/
	/* Encoder Callback Timer Peripheral clock enable */
	ENC_CALLBACK_TIM_CLK_ENABLE();

	/*##-2- Configure the NVIC for Encoder Callback Timer ###########*/
	/* Set the Encoder Callback Timer priority */
	HAL_NVIC_SetPriority(TIM1_BRK_TIM9_IRQn, 3, 0);
	/* Enable the Encoder Callback Timer global Interrupt */
	HAL_NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);


	/* -3- Initialize Encoder Callback Timer */
	/* Initialize Encoder Callback Timer peripheral as follows:
	   + Period = 0
	   + Prescaler = 0
	   + ClockDivision = 0
	   + Counter direction = Up
	*/

	/* Compute the prescaler value to have TIMx counter clock equal to 10000 Hz */
	uwPrescalerValue = (uint32_t)((SystemCoreClock) / Encoder_Callback_Timer_Freq) - 1;

	Encoder_Callback_Handle.Instance 			   = ENC_CALLBACK_TIM;
	Encoder_Callback_Handle.Init.Period            = Encoder_Callback_Timer_Period - 1;
	Encoder_Callback_Handle.Init.Prescaler         = uwPrescalerValue;
	Encoder_Callback_Handle.Init.ClockDivision     = 0;
	Encoder_Callback_Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
	Encoder_Callback_Handle.Init.RepetitionCounter = 0;


	if (HAL_TIM_Base_Init(&Encoder_Callback_Handle) != HAL_OK){
		/* Initialization Error */
		Error_Handler();
	}
}

/**
  * @brief Encoder Timer Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - Handle Configuration
  */
void Encoder_Timer_Init(){
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

	/* -3- Initialize TIM2 to handle the encoder sensor */
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
}

/**
 * Get the distance in m
 * @return distance
 */
float Encoder_GetDistance(){
	return Distance;
}

/**
 * Get the distance in cm
 * @return distance
 */
float Encoder_GetDistance_cm(){
	return 100*Distance;
}

/**
 * Get the velocity
 * @return velocity
 */
float Encoder_GetVelocity(void){
	return Velocity;
}

/**
 * Reset the encoder.
 */
void Encoder_Reset(){
	PrevCnt = 0;
	CurrCnt = 0;
	Distance = 0;
	Velocity = 0;
	ENC_TIM->CNT = 0;
}

/**
 * Callback function for periodical check, and measure velocity in every 50 ms
 */
void Encoder_Callback_Timer(){
	PrevCnt = CurrCnt;
	CurrCnt = ENC_TIM->CNT;
	Distance = METER_PER_INCR*CurrCnt;
	Velocity = (METER_PER_INCR*(CurrCnt - PrevCnt)) * ENC_FREQ;
}
