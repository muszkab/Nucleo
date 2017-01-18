/*
 * Timers.c
 *
 *  Created on: 2016. nov. 1.
 *      Author: Muszka
 */

#include "Timers.h"

/* TIM6_freq=10kHz */
#define MAX_CLOCK_TIM6			90				/* TIM6 max clock: 90MHz */
#define PRESCALER_SZABALYZO		(MAX_CLOCK_TIM6-1)	/* TIM6_freq=90MHZ / (PRESCALER+1) /  (PEROD+1) */
#define PERIOD_SZABALYZO		(1000-1)		/* TIM6_freq=90MHZ / 90 / 1000 = 1kHz */

/* Számlálók 1ms-vel */
uint16_t TimePositionControl=0;
uint16_t TimePositionControl_AT=0;
uint16_t TimeSpeedControl=0;
uint16_t TimeLineType=0;
uint16_t TimeAccelerate=0;
uint16_t TimeBrake=0;
uint16_t TimeSendMessage=0;
uint16_t TimeMotorControl=0;
uint16_t TimeWallType=0;

/* Timer handler declaration */
TIM_HandleTypeDef    TimHandle_Szabalyzo;
TIM_HandleTypeDef    TimHandle_Gyro;

/* Timer megszakításkezezõ callback */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/* TIM6: szabályzó timer, f=1kHZ */
	if(htim->Instance==TIM_SZABALYZO)
	{
		TimePositionControl++;
		TimePositionControl_AT++;
		TimeSpeedControl++;
		TimeLineType++;
		TimeAccelerate++;
		TimeBrake++;
		TimeSendMessage++;
		TimeMotorControl++;
		TimeWallType++;
	}
	else if(htim->Instance == ENC_CALLBACK_TIM){
		Encoder_Callback_Timer();
	}
	else if(htim->Instance==TIM7){
		Gyro_Callback();
	}
}

void Szabalyzo_TIM_Init()
{
	/*## Configure the NVIC for TIMx ########################################*/
	/* Set the TIM_ENC priority */
	HAL_NVIC_SetPriority(TIM_SZABALYZO_IRQn, 4, 0);

	/* Enable the TIMx global Interrupt */
	HAL_NVIC_EnableIRQ(TIM_SZABALYZO_IRQn);

	/*##-1- Enable peripherals #################################*/
	/* TIMx Peripheral clock enable */
	TIM_SZABALYZO_CLK_ENABLE();

	/*##-1- Configure the TIM peripheral #######################################*/
	/* f=1kHz =90MHz/90/1000 */
	/* Initialize TIMx peripheral as follows:
	   + Prescaler = 90 - 1
	   + Period = 1000 - 1
	   + ClockDivision = 1
	   + Counter direction = Up
	*/
	TimHandle_Szabalyzo.Instance = TIM_SZABALYZO;

	TimHandle_Szabalyzo.Init.Prescaler         = PRESCALER_SZABALYZO;
	TimHandle_Szabalyzo.Init.Period            = PERIOD_SZABALYZO;
	TimHandle_Szabalyzo.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
	TimHandle_Szabalyzo.Init.CounterMode       = TIM_COUNTERMODE_UP;
	if (HAL_TIM_Base_Init(&TimHandle_Szabalyzo) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}

	/*##-3- Start timer #######################################*/
	if (HAL_TIM_Base_Start_IT(&TimHandle_Szabalyzo) != HAL_OK)
	{
		/* PWM Generation Error */
		Error_Handler();
	}
}

void GYRO_TIM_Init(){
	/* Prescaler declaration */
	uint32_t uwPrescalerValue = 0;

	/*## Configure the NVIC for TIMx ########################################*/
	/* Set the TIM_ENC priority */
	HAL_NVIC_SetPriority(TIM_GYRO_IRQn, 4, 0);

	/* Enable the TIMx global Interrupt */
	HAL_NVIC_EnableIRQ(TIM_GYRO_IRQn);

	/*##-1- Enable peripherals #################################*/
	/* TIMx Peripheral clock enable */
	TIM_GYRO_CLK_ENABLE();

	/*##-2- Configure the TIM peripheral #######################################*/
	/* f=1kHz =90MHz/90/1000 */
	/* Initialize TIMx peripheral as follows:
	   + Prescaler = 90 - 1
	   + Period = 1000 - 1
	   + ClockDivision = 1
	   + Counter direction = Up
	*/

	/* Compute the prescaler value to have TIMx counter clock equal to 100 Hz */
	uwPrescalerValue = (uint32_t)((SystemCoreClock/2) / GYRO_TIM_FREQ) - 1;

	TimHandle_Gyro.Instance 			  = TIM_GYRO;
	TimHandle_Gyro.Init.Prescaler         = uwPrescalerValue;
	TimHandle_Gyro.Init.Period            = GYRO_TIM_PERIOD -1;
	TimHandle_Gyro.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
	TimHandle_Gyro.Init.CounterMode       = TIM_COUNTERMODE_UP;
	if (HAL_TIM_Base_Init(&TimHandle_Gyro) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}
}
