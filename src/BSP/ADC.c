/*
 * ADC.c
 *
 *  Created on: 2016. dec. 24.
 *      Author: szabgyul92
 */

/* Includes ------------------------------------------------------------------*/
#include "ADC.h"

/* ADC handler declaration */
ADC_HandleTypeDef    Adc1Handle;
/* Timer handler declaration */
TIM_HandleTypeDef    Adc1_Tim_Handle;

/**********************************************/
/***************** TIMER - ADC ****************/
/**********************************************/
#define ADC_TIM_FREQ			10000
#define ADC_TIM_PERIOD			500
#define ADC_TIM_MAX_CLOCK		180				/* TIM8 max clock: 180MHz */
#define ADC_TIM_PRESCALER		((ADC_TIM_MAX_CLOCK*1e6) / ADC_TIM_FREQ) - 1

/**
  * @brief ADC Initialization
  */
void ADC1_Init(){
	ADC_ChannelConfTypeDef 	  sConfig;
	GPIO_InitTypeDef          GPIO_InitStruct;
	static DMA_HandleTypeDef  Dma2Handle;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* ADC1 Periph clock enable */
	ADC1_CLK_ENABLE();
	/* Enable GPIO clock ****************************************/
	ADC1_CHANNEL4_GPIO_CLK_ENABLE();
	ADC1_CHANNEL10_GPIO_CLK_ENABLE();
	ADC1_CHANNEL13_GPIO_CLK_ENABLE();
	ADC1_CHANNEL14_GPIO_CLK_ENABLE();
	/* Enable DMA2 clock */
	DMA2_CLK_ENABLE();

	/*##-2- Configure peripheral GPIO ##########################################*/
	/* ADC Channel GPIO pin configuration */
	GPIO_InitStruct.Pin = ADC1_CHANNEL4_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(ADC1_CHANNEL4_GPIO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = ADC1_CHANNEL10_PIN;
	HAL_GPIO_Init(ADC1_CHANNEL10_GPIO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = ADC1_CHANNEL13_PIN;
	HAL_GPIO_Init(ADC1_CHANNEL13_GPIO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = ADC1_CHANNEL14_PIN;
	HAL_GPIO_Init(ADC1_CHANNEL14_GPIO_PORT, &GPIO_InitStruct);

	/*##-3- Configure the DMA streams ##########################################*/
	/* Set the parameters to be configured */
	Dma2Handle.Instance = ADC1_DMA_STREAM;
	Dma2Handle.Init.Channel  = ADC1_DMA_CHANNEL;
	Dma2Handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
	Dma2Handle.Init.PeriphInc = DMA_PINC_DISABLE;
	Dma2Handle.Init.MemInc = DMA_MINC_ENABLE;
	Dma2Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; //TODO fél szó
	Dma2Handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	Dma2Handle.Init.Mode = DMA_CIRCULAR;
	Dma2Handle.Init.Priority = DMA_PRIORITY_HIGH;
	Dma2Handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	Dma2Handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
	Dma2Handle.Init.MemBurst = DMA_MBURST_SINGLE;
	Dma2Handle.Init.PeriphBurst = DMA_PBURST_SINGLE;

	HAL_DMA_Init(&Dma2Handle);

	/* Associate the initialized DMA handle to the ADC handle */
	__HAL_LINKDMA(&Adc1Handle, DMA_Handle, Dma2Handle);

	/*##-4- Configure the NVIC for DMA #########################################*/
	/* NVIC configuration for DMA transfer complete interrupt */
	HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 7, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

	/*##-2- Configure the ADC peripheral #######################################*/
	Adc1Handle.Instance                   = ADC1;
	Adc1Handle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;
	Adc1Handle.Init.Resolution            = ADC_RESOLUTION;
	Adc1Handle.Init.ScanConvMode          = ENABLE;                         /* Sequencer enabled (ADC conversion on multiple channels) */
	Adc1Handle.Init.ContinuousConvMode    = DISABLE;                        /* Continuous mode disabled to have only 1 conversion at each conversion trig */
	Adc1Handle.Init.DiscontinuousConvMode = DISABLE;                        /* Parameter discarded because sequencer is disabled */
	Adc1Handle.Init.NbrOfDiscConversion   = 0;
	Adc1Handle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;  /* Conversion start trigged at each external event */
	Adc1Handle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T8_TRGO;
	Adc1Handle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
	Adc1Handle.Init.NbrOfConversion       = 4;
	Adc1Handle.Init.DMAContinuousRequests = ENABLE;
	Adc1Handle.Init.EOCSelection          = DISABLE;

	if (HAL_ADC_Init(&Adc1Handle) != HAL_OK){
		/* ADC initialization Error */
		Error_Handler();
	}

	/*##-3- Configure ADC regular channels ######################################*/
	/***************** Battery Feedback ******************/
	sConfig.Channel      = ADC1_CHANNEL10;
	sConfig.Rank         = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME;
	sConfig.Offset       = 0;

	if (HAL_ADC_ConfigChannel(&Adc1Handle, &sConfig) != HAL_OK){
		/* Channel Configuration Error */
		Error_Handler();
	}

	/* Front Sharp */
	sConfig.Channel      = ADC1_CHANNEL4;
	sConfig.Rank         = 2;
	sConfig.SamplingTime = ADC_SAMPLETIME;
	sConfig.Offset       = 0;

	if (HAL_ADC_ConfigChannel(&Adc1Handle, &sConfig) != HAL_OK){
		/* Channel Configuration Error */
		Error_Handler();
	}

	/* Left Sharp */
	sConfig.Channel      = ADC1_CHANNEL13;
	sConfig.Rank         = 3;
	sConfig.SamplingTime = ADC_SAMPLETIME;
	sConfig.Offset       = 0;

	if (HAL_ADC_ConfigChannel(&Adc1Handle, &sConfig) != HAL_OK){
		/* Channel Configuration Error */
		Error_Handler();
	}

	/* Right Sharp */
	sConfig.Channel      = ADC1_CHANNEL14;
	sConfig.Rank         = 4;
	sConfig.SamplingTime = ADC_SAMPLETIME;
	sConfig.Offset       = 0;

	if (HAL_ADC_ConfigChannel(&Adc1Handle, &sConfig) != HAL_OK){
		/* Channel Configuration Error */
		Error_Handler();
	}
}

/**
  * @brief ADC Initialization
  */
void ADC1_TIM_Init(){
	TIM_MasterConfigTypeDef sMasterConfig;

	/*##-1- Enable peripheral clock #################################*/
	/* TIM8 Peripheral clock enable */
	ADC_TIM_CLK_ENABLE();

	//TODO debug miatt az IT-ben LED villogtatással ellenõrizhetõ a frekvencia
	/*##-2- Configure the NVIC for TIM3 ########################################*/
	/* Set the TIM3 priority */
	HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 7, 0);
	/* Enable the TIM3 global Interrupt */
	HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);

	/* Time Base configuration */
	Adc1_Tim_Handle.Instance 			   = TIM8;
	Adc1_Tim_Handle.Init.Period            = ADC_TIM_PERIOD;
	Adc1_Tim_Handle.Init.Prescaler         = ADC_TIM_PRESCALER;
	Adc1_Tim_Handle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
	Adc1_Tim_Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
	Adc1_Tim_Handle.Init.RepetitionCounter = 0;

	if (HAL_TIM_Base_Init(&Adc1_Tim_Handle) != HAL_OK){
		/* Timer initialization Error */
		Error_Handler();
	}

	/* Timer TRGO selection */
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

	if (HAL_TIMEx_MasterConfigSynchronization(&Adc1_Tim_Handle, &sMasterConfig) != HAL_OK){
		/* Timer TRGO selection Error */
		Error_Handler();
	}
}
