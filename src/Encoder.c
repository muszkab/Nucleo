/*
 * Encoder.c
 *
 *  Created on: 2016. okt. 26.
 *      Author: Muszka
 */

#include "Encoder.h"

/* Definition for TIM_ENC clock resources */
#define TIM_ENC                           TIM2
#define TIM_ENC_CLK_ENABLE()              __HAL_RCC_TIM2_CLK_ENABLE()

/* Definition for TIM_ENC Channel Pins */
#define TIM_ENC_CHANNEL_GPIO_PORT()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define TIM_ENC_GPIO_PORT			       GPIOA
#define TIM_ENC_GPIO_PIN_CHANNEL1         GPIO_PIN_0
#define TIM_ENC_GPIO_PIN_CHANNEL2         GPIO_PIN_1
#define TIM_ENC_GPIO_ALTERNATE_FUNCTION   GPIO_AF1_TIM2

/* Ha a f�lperi�dus hossza t�l nagy, van egy fels� limit. Ez a sebess�g�rt�k �rt�k, ha �ll az aut� */
#define MAX_HALFPERIOD		(uint16_t)5000

/* Private variables ---------------------------------------------------------*/
/* Timer handler declaration */
TIM_HandleTypeDef    TimHandle_Encoder;
TIM_HandleTypeDef    TimHandle_HalfPeriod;

/* Timer Output Compare Configuration Structure declaration */
TIM_Encoder_InitTypeDef sConfig_Encoder;

/* Count, speed variables */
/* Enk�der sz�ml�l� �rt�kekb�l sz�molt mennyis�gek*/
static volatile int32_t count=0;
static volatile int32_t prevCount=0;
static volatile int32_t 		speed=0;
volatile int32_t		distance=0;
/* F�lperi�dusid� m�r�s */
/* F�lperi�dus hossza, darabsz�m */
uint16_t		countHalfPeriod=0;
/* Sebess�g: kisebb sz�m�rt�k -> nagyobb sebess�g�rt�k */
volatile uint16_t		speedHalfPeriod=0;


void Encoder_Init()
{
	GPIO_InitTypeDef   GPIO_InitStruct;
	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* TIM_ENC Peripheral clock enable */
	TIM_ENC_CLK_ENABLE();
	/* Enable all GPIO Channels Clock requested */
	TIM_ENC_CHANNEL_GPIO_PORT();
	/* Configure PA0, PA1 in push-pull, alternate function mode*/
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

	GPIO_InitStruct.Alternate = TIM_ENC_GPIO_ALTERNATE_FUNCTION;
	GPIO_InitStruct.Pin = TIM_ENC_GPIO_PIN_CHANNEL1 | TIM_ENC_GPIO_PIN_CHANNEL2;
	HAL_GPIO_Init(TIM_ENC_GPIO_PORT, &GPIO_InitStruct);

	  /* -2- Initialize TIM to handle the encoder sensor */
	  /* Initialize TIM peripheral as follow:
		   + Period = 65535
		   + Prescaler = 0
		   + ClockDivision = 0
		   + Counter direction = Up
		N�gyszeres ki�rt�kel�s: TIM_ENCODERMODE_TI12, TIM_ICPOLARITY_BOTHEDGE
	  */
	  TimHandle_Encoder.Instance = TIM_ENC;

	  TimHandle_Encoder.Init.Period            = 65535;
	  TimHandle_Encoder.Init.Prescaler         = 0;
	  TimHandle_Encoder.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
	  TimHandle_Encoder.Init.CounterMode       = TIM_COUNTERMODE_UP;
	  TimHandle_Encoder.Init.RepetitionCounter = 0;

	  sConfig_Encoder.EncoderMode        = TIM_ENCODERMODE_TI12;

	  sConfig_Encoder.IC1Polarity        = TIM_ICPOLARITY_BOTHEDGE;
	  sConfig_Encoder.IC1Selection       = TIM_ICSELECTION_DIRECTTI;
	  sConfig_Encoder.IC1Prescaler       = TIM_ICPSC_DIV1;
	  sConfig_Encoder.IC1Filter          = 0;

	  sConfig_Encoder.IC2Polarity        = TIM_ICPOLARITY_BOTHEDGE;
	  sConfig_Encoder.IC2Selection       = TIM_ICSELECTION_DIRECTTI;
	  sConfig_Encoder.IC2Prescaler       = TIM_ICPSC_DIV1;
	  sConfig_Encoder.IC2Filter          = 0;

	  if(HAL_TIM_Encoder_Init(&TimHandle_Encoder, &sConfig_Encoder) != HAL_OK)
	  {
		/* Initialization Error */
		Error_Handler();
	  }

	  /*## Configure the NVIC for TIM_ENC ########################################*/
	  /* Set the TIM_ENC priority */
	  HAL_NVIC_SetPriority(TIM2_IRQn, 5, 0);

	  /* Enable the TIM_ENC global Interrupt */
	  HAL_NVIC_EnableIRQ(TIM2_IRQn);

	  /* Seg�dtimer a f�lperi�dus m�r�shez */
	  Timer11_Init();

	  /* TODO: eg�sz algoritmust �tgondolni, start fgv k�l�n */
	  /* Start the encoder interface */
	  /* F�lperi�dusid� m�r�s miatt IT, mert A csatorna minden �lv�lt�sn�l
	   * a megszak�t�skezel�ben megh�vja Encoder_ReadHalfPeriod fgv-t */
	  HAL_TIM_Encoder_Start_IT(&TimHandle_Encoder, TIM_CHANNEL_1);
	  /* B csatorna sim�n */
	  HAL_TIM_Encoder_Start(&TimHandle_Encoder, TIM_CHANNEL_2);

}

/* Sebess�gm�r�s k�t sz�ml�l��rt�k(count) k�l�nbs�ge alapj�n, periodikusan megh�vva
 * (sebess�gsz�m�t�s peri�dusiedeje minimum 1ms)
 * hiba: 46% 1km/h-n�l*/
void Encoder_ReadCount()
{
	prevCount	= count;
	count		= __HAL_TIM_GetCounter(&TimHandle_Encoder);
	speed		= count - prevCount;
	distance	+= speed;

}

/* Sebbess�gm�r�s egy csatorna f�lperi�dus idej�nek m�r�s�vel,
 * seg�dfrekvencia: f=2MHz, hiba: 1% 20km/h-n�l */
void Encoder_ReadHalfPeriod()
{
	/* Ha �ll az aut�, akkor elvileg countHalfPeriod v�gtelen, erre kell egy limit: MAX_HALFPERIOD */
	if(countHalfPeriod < MAX_HALFPERIOD)
		speedHalfPeriod = countHalfPeriod;
	else
		speedHalfPeriod = MAX_HALFPERIOD;

	countHalfPeriod = 0;
}

/* V�ltoz�k null�z�sa */
void Encoder_VariablesReset()
{
	count = 0;
	prevCount = 0;
	speed = 0;
	distance = 0;
	__HAL_TIM_SET_COUNTER(&TimHandle_Encoder, 0);

	speedHalfPeriod = 0;
	countHalfPeriod = 0;
}

/* TIM11: seg�dfrekvencia a f�lperi�dusid� m�r�shez, f=2MHz */
void Timer11_Init()
{
	  /*##-1- Enable peripheral clock #################################*/
	  /* TIM11 Peripheral clock enable */
	  __HAL_RCC_TIM11_CLK_ENABLE();

	  /*##-2- Configure the NVIC for TIM11 ########################################*/
	  /* Set the TIM11 priority */
	  HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 6, 0);

	  /* Enable the TIM11 global Interrupt */
	  HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);

	  /* Set TIM11 instance */
	  TimHandle_HalfPeriod.Instance = TIM11;

	  /* Initialize TIM_ENC peripheral as follows:
	   * 	f = 2MHz = 180MHz / (Prescaler+1) / (Period+1) = 180 / 90 = 2MHz
	       + Period = 90-1
	       + Prescaler = 0
	       + ClockDivision = 0
	       + Counter direction = Up
	  */
	  /* TODO: period �rt�k!! (t�l nagy f blokkolja a procit, de j� lenne 2MHZ*/
	  TimHandle_HalfPeriod.Init.Period            = 899;
	  TimHandle_HalfPeriod.Init.Prescaler         = 0;
	  TimHandle_HalfPeriod.Init.ClockDivision     = 0;
	  TimHandle_HalfPeriod.Init.CounterMode       = TIM_COUNTERMODE_UP;
	  TimHandle_HalfPeriod.Init.RepetitionCounter = 0;

	  if (HAL_TIM_Base_Init(&TimHandle_HalfPeriod) != HAL_OK)
	  {
	    /* Initialization Error */
	    Error_Handler();
	  }

	  /*##-3- Start the TIM Base generation in interrupt mode ####################*/
	  /* Start Channel1 */
	  if (HAL_TIM_Base_Start_IT(&TimHandle_HalfPeriod) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

/*
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	// TIM_ENC: A csatorna �lv�lt�s, f�lperi�dus v�ge
	if(htim->Instance==TIM_ENC)
	{
		Encoder_ReadHalfPeriod();
	}
}
*/
