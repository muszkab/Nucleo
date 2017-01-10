/*
 * RemoteControl.c
 *
 *  Created on: 2016. nov. 27.
 *      Author: Muszka
 */

#include "RemoteControl.h"

#define PRESCALER	(180-1)
#define PERIOD		0xFFFF

/* Timer handler declaration */
TIM_HandleTypeDef    TimHandle_Remote;

/* Timer Input Capture Configuration Structure declaration */
static TIM_IC_InitTypeDef       sConfig;

/* Slave configuration structure */
static TIM_SlaveConfigTypeDef   sSlaveConfig;

/* Captured Value */
__IO uint32_t            uwIC2Value = 0;
/* Duty Cycle Value */
__IO uint32_t            uwDutyCycle = 0;
/* Frequency Value */
__IO uint32_t            uwFrequency = 0;

void RemoteController_Timer_Init()
{
	  GPIO_InitTypeDef   GPIO_InitStruct;

	  /*##-1- Enable peripherals and GPIO Clocks #################################*/
	  /* TIMx Peripheral clock enable */
	  TIM_REMOTE_CLK_ENABLE();

	  /* Enable GPIO channels Clock */
	  TIM_REMOTE_CHANNEL_GPIO_PORT();

	  /* Configure  (TIMx_Channel) in Alternate function, push-pull and High speed */
	  GPIO_InitStruct.Pin = TIM_REMOTE_GPIO_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	  GPIO_InitStruct.Alternate = TIM_REMOTE_GPIO_AF;
	  HAL_GPIO_Init(TIM_REMOTE_GPIO_PORT, &GPIO_InitStruct);

	  /*##-2- Configure the NVIC for TIMx #########################################*/

	  HAL_NVIC_SetPriority(TIM_REMOTE_CC_IRQn, 0, 1);

	  /* Enable the TIMx global Interrupt */
	  HAL_NVIC_EnableIRQ(TIM_REMOTE_CC_IRQn);


	  /*##-1- Configure the TIM peripheral #######################################*/
	  /* ---------------------------------------------------------------------------
	  TIM1 configuration: PWM Input mode


	  TIM1 freq = 1MHz -> Prescaler=(180-1)

	  External Signal Frequency = TIM1 counter clock / TIM1_CCR2 in Hz.

	  External Signal DutyCycle = (TIM1_CCR1*1000)/(TIM1_CCR2) in xx.x%.

	  The minimum frequency value to measure is (TIM1 counter clock / CCR MAX)
                                         = (1 MHz)/ 65535
                                         = 15.259 Hz

	  --------------------------------------------------------------------------- */

	  /* Set TIMx instance */
	  TimHandle_Remote.Instance = TIM_REMOTE;

	  /* Initialize TIMx peripheral as follows:
	       + Period = 0xFFFF
	       + Prescaler = 180
	       + ClockDivision = 0
	       + Counter direction = Up
	  */
	  TimHandle_Remote.Init.Period            = 0xFFFF;
	  TimHandle_Remote.Init.Prescaler         = (180-1);
	  TimHandle_Remote.Init.ClockDivision     = 0;
	  TimHandle_Remote.Init.CounterMode       = TIM_COUNTERMODE_UP;
	  TimHandle_Remote.Init.RepetitionCounter = 0;
	  if (HAL_TIM_IC_Init(&TimHandle_Remote) != HAL_OK)
	  {
	    /* Initialization Error */
	    Error_Handler();
	  }

	  /*##-2- Configure the Input Capture channels ###############################*/
	  /* Common configuration */
	  sConfig.ICPrescaler = TIM_ICPSC_DIV1;
	  sConfig.ICFilter = 0;

	  /* Configure the Input Capture of channel 1 */
	  sConfig.ICPolarity = TIM_ICPOLARITY_FALLING;
	  sConfig.ICSelection = TIM_ICSELECTION_INDIRECTTI;
	  if (HAL_TIM_IC_ConfigChannel(&TimHandle_Remote, &sConfig, TIM_CHANNEL_1) != HAL_OK)
	  {
	    /* Configuration Error */
	    Error_Handler();
	  }

	  /* Configure the Input Capture of channel 2 */
	  sConfig.ICPolarity = TIM_ICPOLARITY_RISING;
	  sConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
	  if (HAL_TIM_IC_ConfigChannel(&TimHandle_Remote, &sConfig, TIM_CHANNEL_2) != HAL_OK)
	  {
	    /* Configuration Error */
	    Error_Handler();
	  }
	  /*##-3- Configure the slave mode ###########################################*/
	  /* Select the slave Mode: Reset Mode  */
	  sSlaveConfig.SlaveMode        = TIM_SLAVEMODE_RESET;
	  sSlaveConfig.InputTrigger     = TIM_TS_TI2FP2;
	  sSlaveConfig.TriggerPolarity  = TIM_TRIGGERPOLARITY_NONINVERTED;
	  sSlaveConfig.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1;
	  sSlaveConfig.TriggerFilter    = 0;
	  if (HAL_TIM_SlaveConfigSynchronization(&TimHandle_Remote, &sSlaveConfig) != HAL_OK)
	  {
	    /* Configuration Error */
	    Error_Handler();
	  }

	  /*##-4- Start the Input Capture in interrupt mode ##########################*/
	  if (HAL_TIM_IC_Start_IT(&TimHandle_Remote, TIM_CHANNEL_2) != HAL_OK)
	  {
	    /* Starting Error */
	    Error_Handler();
	  }

	  /*##-5- Start the Input Capture in interrupt mode ##########################*/
	  if (HAL_TIM_IC_Start_IT(&TimHandle_Remote, TIM_CHANNEL_1) != HAL_OK)
	  {
	    /* Starting Error */
	    Error_Handler();
	  }
}

/* Sebess�gjel a t�vir�ny�t� alapj�n */
void SetSpeed_RemoteControl()
{
	//sebess�gjel
	static int8_t value=0;
	//zajsz�r�shez id�m�r�s
	static uint8_t temp=0;

	//uwDutyCycle: 57..140 k�z�tt.
	//value: -20..21(21.5) k�z�tt
	if(uwDutyCycle < 150 && uwDutyCycle > 50)	//kit�lt�si t�nyez� �rt�k vizsg�lat, tartom�nyon bel�l legyen
		value = (uwDutyCycle-97)>>1;			//sebess�gjel konverz� a kit�lt�si t�nyez�b�l: 57..140 -> -20..21.5
	else
		value = 0;
	if(value<4 && value>-4)	//holtj�t�k a t�vir�ny�t�nak
		value=0;

	//t�vir�ny�t� zaj sz�r�s
	if(value==0)	//ha nulla a sebess�g, el�lr�l kezdj�k az id�m�r�st
		temp=0;
	else
		temp++;
	//nem kell neki nagyon sokaig sz�molni, t�lcsordul�s ellen is
	if(temp>20)
		temp=20;

	//csak akkor k�ldj�n sebess�gjelet, ha egy ideje(temp �rt�ke) m�r veszi a jelet a t�vir�ny�t�t�l,
	//de a nulla �rt�ket mindig k�ldeni kell neki, hogy ne az el�z� �rt�k ragadjon be, mivel value==0 eset�n temp is nulla
	if(temp>5 || value==0)
	{
		SetSpeed(2*value*10);
		temp=0;
	}
}

void Is_StopCommand()
{
	if(uwDutyCycle<70)
	{
		StateQ1 = Stop;
	}
}

void RemoteController_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    /* Get the Input Capture value */
    uwIC2Value = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);

    if (uwIC2Value != 0)
    {
      /* Duty cycle computation */
      uwDutyCycle = ((HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1)) * 1000) / uwIC2Value;

      /* uwFrequency computation
      TIM1 counter clock = 1MHz */
      uwFrequency = 1000000  / uwIC2Value;
    }
    else
    {
      uwDutyCycle = 0;
      uwFrequency = 0;
    }
}

/**
  * @brief  Input Capture callback in non blocking mode
  * @param  htim : TIM IC handle
  * @retval None
  */
//timers.c-be �trakni ha m�s timer is ker�l bele!
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2 && htim->Instance == TIM_REMOTE)
  {
	  RemoteController_TIM_IC_CaptureCallback(htim);
  }
}
