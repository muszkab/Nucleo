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

#define BufferSize 1000
uint16_t uhADC1ConvertedValues[BufferSize];

/**
  * @brief ADC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc){
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
	HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0); //TODO prioritás
	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

/**
  * @brief ADC Initialization
  */
void ADC1_Init(){
	ADC_ChannelConfTypeDef sConfig;

	/*##-1- Configure the ADC peripheral #######################################*/
	Adc1Handle.Instance                   = ADC1;
	Adc1Handle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;
	Adc1Handle.Init.Resolution            = ADC_RESOLUTION;
	Adc1Handle.Init.ScanConvMode          = ENABLE;                         /* Sequencer enabled (ADC conversion on multiple channels) */
	Adc1Handle.Init.ContinuousConvMode    = DISABLE;                        /* Continuous mode disabled to have only 1 conversion at each conversion trig TODO engedélyezni lehetne*/
	Adc1Handle.Init.DiscontinuousConvMode = DISABLE;                        /* Parameter discarded because sequencer is disabled */
	Adc1Handle.Init.NbrOfDiscConversion   = 0;
	Adc1Handle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;  /* Conversion start trigged at each external event */
	Adc1Handle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;	/*TODO hardveres triggerrel meg lehetne oldani? */
	Adc1Handle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
	Adc1Handle.Init.NbrOfConversion       = 4;
	Adc1Handle.Init.DMAContinuousRequests = ENABLE;
	Adc1Handle.Init.EOCSelection          = DISABLE;

	if (HAL_ADC_Init(&Adc1Handle) != HAL_OK){
		/* ADC initialization Error */
		Error_Handler();
	}

	//TODO sorrendet majd le kell ellenõrizni
	/*##-2- Configure ADC regular channels ######################################*/
	sConfig.Channel      = ADC1_CHANNEL4;
	sConfig.Rank         = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME;
	sConfig.Offset       = 0;

	if (HAL_ADC_ConfigChannel(&Adc1Handle, &sConfig) != HAL_OK){
		/* Channel Configuration Error */
		Error_Handler();
	}


	/***************** Battery Feedback ******************/
	sConfig.Channel      = ADC1_CHANNEL10;
	sConfig.Rank         = 2;
	sConfig.SamplingTime = ADC_SAMPLETIME;
	sConfig.Offset       = 0;

	if (HAL_ADC_ConfigChannel(&Adc1Handle, &sConfig) != HAL_OK){
		/* Channel Configuration Error */
		Error_Handler();
	}

	sConfig.Channel      = ADC1_CHANNEL13;
	sConfig.Rank         = 3;
	sConfig.SamplingTime = ADC_SAMPLETIME;
	sConfig.Offset       = 0;

	if (HAL_ADC_ConfigChannel(&Adc1Handle, &sConfig) != HAL_OK){
		/* Channel Configuration Error */
		Error_Handler();
	}

	sConfig.Channel      = ADC1_CHANNEL14;
	sConfig.Rank         = 4;
	sConfig.SamplingTime = ADC_SAMPLETIME;
	sConfig.Offset       = 0;

	if (HAL_ADC_ConfigChannel(&Adc1Handle, &sConfig) != HAL_OK){
		/* Channel Configuration Error */
		Error_Handler();
	}

	//TODO startolni nem biztos, h itt kell
	/*##-3- Start the conversion process #######################################*/
	/* Note: Considering IT occurring after each number of ADC conversions      */
	/*       (IT by DMA end of transfer), select sampling time and ADC clock    */
	/*       with sufficient duration to not create an overhead situation in    */
	/*        IRQHandler. */
	if(HAL_ADC_Start_DMA(&Adc1Handle, (uint32_t*)&uhADC1ConvertedValues, 4) != HAL_OK){
		/* Start Conversation Error */
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

}

