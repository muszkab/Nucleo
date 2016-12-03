/*
 * Timers.c
 *
 *  Created on: 2016. nov. 1.
 *      Author: Muszka
 */

#include "Timers.h"

/* TIM6_freq=10kHz */
#define MAX_CLOCK		180				/* TIM6 max clock: 180MHz */
#define PRESCALER		(MAX_CLOCK-1)	/* TIM6_freq=180MHZ / (PRESCALER+1) /  (PEROD+1) */
#define PERIOD			(100-1)			/* TIM6_freq=180MHZ / (180) / (100)=10kHz */

/* Számláló 0.1ms-vel */
uint32_t Szabcount=0;

/* Timer handler declaration */
TIM_HandleTypeDef    TimHandle_Szabalyzo;

/* Definition for TIMx clock resources */
#define TIM_SZABALYZO                           TIM6
#define TIM_SZABALYZO_CLK_ENABLE()              __HAL_RCC_TIM6_CLK_ENABLE()
#define TIM_SZABALYZO_IRQn						TIM6_DAC_IRQn

/* Definition for TIMx Channel Pins
#define TIMx_CHANNEL_GPIO_PORT()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define TIMx_GPIO_PORT_CHANNEL1        GPIOB
#define TIMx_GPIO_PIN_CHANNEL1         GPIO_PIN_8
#define TIMx_GPIO_AF_CHANNEL1          GPIO_AF3_TIM10
*/

void Szabalyzo_TIM_Init()
{
	  /*## Configure the NVIC for TIM_ENC ########################################*/
	  /* Set the TIM_ENC priority */
	  HAL_NVIC_SetPriority(TIM_SZABALYZO_IRQn, 4, 0);

	  /* Enable the TIM_ENC global Interrupt */
	  HAL_NVIC_EnableIRQ(TIM_SZABALYZO_IRQn);

	/*##-1- Enable peripherals #################################*/
	/* TIMx Peripheral clock enable */
	TIM_SZABALYZO_CLK_ENABLE();

	  /*##-1- Configure the TIM peripheral #######################################*/
	  /* f=10kHz =180MHz/180/100 */
	  /* Initialize TIMx peripheral as follows:
		   + Prescaler = 180 - 1
		   + Period = 100 - 1
		   + ClockDivision = 1
		   + Counter direction = Up
	  */
	TimHandle_Szabalyzo.Instance = TIM_SZABALYZO;

	TimHandle_Szabalyzo.Init.Prescaler         = PRESCALER;
	TimHandle_Szabalyzo.Init.Period            = PERIOD;
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

/* Timer megszakításkezezõ callback */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/* TIM11: enkóder félperiódus mérés, f=2MHz, számláló növelés */
	if(htim->Instance==TIM11)
	{
	  countHalfPeriod++;
	}
	/* TIM6: szabályzó timer, f=10kHZ */
	if(htim->Instance==TIM_SZABALYZO)
	{
		Szabcount++;
	}
}
