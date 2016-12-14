/*
 * ADC.c
 *
 *  Created on: 2016. szept. 25.
 *      Author: Muszka
 */

#include "ADC.h"

/* Definition for TIM_ADC clock resources */
#define TIM_ADC                           TIM4
#define TIM_ADC_CLK_ENABLE()              __HAL_RCC_TIM4_CLK_ENABLE()
#define TIM_ADC_IRQN					  TIM4_IRQn

/* ADC handler declaration */
ADC_HandleTypeDef    AdcHandle;

/* Timer handler declaration */
TIM_HandleTypeDef    TimHandle_ADC;

/*  */
volatile uint16_t uhADCxConvertedValue = 0;

void ADC1_Init()
{

	ADC_ChannelConfTypeDef sConfig;
	GPIO_InitTypeDef	GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	  /* ADC1 Periph clock enable */
	  ADC1_CLK_ENABLE();
	  /* Enable GPIO clock ****************************************/
	  ADC1_CHANNEL_GPIO_CLK_ENABLE();

	  /*##-2- Configure peripheral GPIO ##########################################*/
	  /* ADC Channel GPIO pin configuration */
	  GPIO_InitStruct.Pin = ADC1_CHANNEL1_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(ADC1_CHANNEL1_GPIO_PORT, &GPIO_InitStruct);

	  GPIO_InitStruct.Pin = ADC1_CHANNEL2_PIN;
	  HAL_GPIO_Init(ADC1_CHANNEL2_GPIO_PORT, &GPIO_InitStruct);

	  GPIO_InitStruct.Pin = ADC1_CHANNEL3_PIN;
	  HAL_GPIO_Init(ADC1_CHANNEL3_GPIO_PORT, &GPIO_InitStruct);

	  /*##-3- Configure the NVIC #################################################*/
	  /* NVIC configuration for ADC interrupt */
	  HAL_NVIC_SetPriority(ADC1_IRQn, 2, 0);
	  HAL_NVIC_EnableIRQ(ADC1_IRQn);

	/*##-1- Configure the ADC peripheral #######################################*/
	  AdcHandle.Instance          = ADC1;


	  AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;
	  AdcHandle.Init.Resolution            = ADC_RESOLUTION12b;
	  AdcHandle.Init.ScanConvMode          = ENABLE;                         /* Sequencer enable (ADC conversion on 3 channel) */
	  AdcHandle.Init.ContinuousConvMode    = DISABLE;                        /* Continuous mode disabled to have only 1 conversion at each conversion trig */
	  AdcHandle.Init.DiscontinuousConvMode = DISABLE;                        /* Parameter discarded because sequencer is disabled */
	  AdcHandle.Init.NbrOfDiscConversion   = 0;
	  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;/* Conversion start trigged at each external event */
	  AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T4_CC4;	 /* TIM4 rising edge */
	  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
	  AdcHandle.Init.NbrOfConversion       = 3;								 /* 3 channel */
	  AdcHandle.Init.DMAContinuousRequests = DISABLE;
	  AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;			 /* Minden egyes csatorna konverzió után megszakítás */

	  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
	  {
		/* ADC initialization Error */
		Error_Handler();
	  }

	  /*##-2- Configure ADC regular channel ######################################*/
	  sConfig.Channel      = ADC_CHANNEL_3;
	  sConfig.Rank         = 1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	  sConfig.Offset       = 0;
	  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
	  {
		/* Channel Configuration Error */
		Error_Handler();
	  }

	  sConfig.Channel      = ADC_CHANNEL_4;
	  sConfig.Rank         = 2;
	  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	  sConfig.Offset       = 0;
	  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
	  {
		/* Channel Configuration Error */
		Error_Handler();
	  }

	  sConfig.Channel      = ADC_CHANNEL_5;
	  sConfig.Rank         = 3;
	  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	  sConfig.Offset       = 0;
	  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
	  {
		/* Channel Configuration Error */
		Error_Handler();
	  }


	  /*##-3- Start the conversion process #######################################*/
	  if (HAL_ADC_Start_IT(&AdcHandle) != HAL_OK)
	  {
		Error_Handler();
	  }

	  /* Init and start ADC_timer */
	  TimerADC_Init();
}

/* Trigger az ADC-nek, f=100Hz */
void TimerADC_Init()
{
	TIM_MasterConfigTypeDef sMasterConfig;

	  /*##-1- Enable peripheral clock #################################*/
	  /* TIM Peripheral clock enable */
	  TIM_ADC_CLK_ENABLE();

	  /*##-2- Configure the NVIC for TIM ########################################*/
	  /*Ugyanaz a prioritás, mint ADC-nek */
	  HAL_NVIC_SetPriority(TIM_ADC_IRQN, 2, 0);

	  /* Enable the TIM global Interrupt */
	  HAL_NVIC_EnableIRQ(TIM_ADC_IRQN);

	  /* Set TIM instance */
	  TimHandle_ADC.Instance = TIM_ADC;

	  /* Initialize TIM peripheral as follows:
	   * 	f = 100Hz = 180MHz / (Period+1) / (Prescaler+1) = 18*10^7 / (18*10^3) / 10^2 = 10^2 Hz
	       + Period = 18000-1
	       + Prescaler = 100-1
	       + ClockDivision = 0
	       + Counter direction = Up
	  */
	  /* TODO: nem 180Mhz a max vagy igen??*/
	  TimHandle_ADC.Init.Period            = 17999;
	  TimHandle_ADC.Init.Prescaler         = 99;
	  TimHandle_ADC.Init.ClockDivision     = 0;
	  TimHandle_ADC.Init.CounterMode       = TIM_COUNTERMODE_UP;
	  TimHandle_ADC.Init.RepetitionCounter = 0;

	  if (HAL_TIM_Base_Init(&TimHandle_ADC) != HAL_OK)
	  {
	    /* Initialization Error */
	    Error_Handler();
	  }

	  /* Timer TRGO selection */
	  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

	  if (HAL_TIMEx_MasterConfigSynchronization(&TimHandle_ADC, &sMasterConfig) != HAL_OK)
	  {
	    /* Timer TRGO selection Error */
	    Error_Handler();
	  }

	  /*##-3- Start the TIM Base generation in interrupt mode ####################*/
	  /* TODO: máshol meghívni? */
	  /* Start ADC timer */
	  if (HAL_TIM_Base_Start(&TimHandle_ADC) != HAL_OK)
	  {
	    /* Starting Error */
	    Error_Handler();
	  }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
  /* Get the converted value of regular channel */
  uhADCxConvertedValue = HAL_ADC_GetValue(AdcHandle);
  //if(uhADCxConvertedValue<2000)
	  //BSP_LED_On(LED2);
}
