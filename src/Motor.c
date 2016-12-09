/*
 * Motor.c
 *
 *  Created on: 2016. nov. 22.
 *      Author: Muszka
 */

#include "Motor.h"

#define MAX_SPEED		35	//maximum: 45
#define START_DUTYCCLE1 50
#define START_DUTYCCLE2 50

/* TIM3 - PWM */
#define MAX_CLOCK		90				/* TIM3 max clock: 90MHz */
#define PRESCALER		(4-1)			/* TIM_freq=MAX_CLOCK*10^6/(Prescaler+1) */ //TIM3_freq=90MHZ/4 =22.5MHz
#define PERIOD			(500-1)			/* PWM_frequency=TIM_freq/(Period+1)=22.5MHZ/500/2=22.5kHZ
										 * 500/2: center aligned pwm miatt máshogy van a PERIOD érték*/
#define DUTY_1percent	(PERIOD+1)/100	/* 1%-os kitöltési tényezõ */


/* TIM3_CH2:	PA7
 * TIM3_CH1:	PC6
 * PWM freq:	20kHz */
/* Definition for TIMx clock resources */
#define TIM_MOTOR                           TIM3
#define TIM_MOTOR_CLK_ENABLE()              __HAL_RCC_TIM3_CLK_ENABLE()

/* Definition for TIMx Channel Pins  */
#define TIM_MOTOR_CHANNEL1_GPIO_PORT()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define TIM_MOTOR_CHANNEL2_GPIO_PORT()       __HAL_RCC_GPIOC_CLK_ENABLE()
#define TIM_MOTOR_GPIO_PORT_CHANNEL1        GPIOA
#define TIM_MOTOR_GPIO_PORT_CHANNEL2        GPIOC
#define TIM_MOTOR_GPIO_PIN_CHANNEL1         GPIO_PIN_7
#define TIM_MOTOR_GPIO_PIN_CHANNEL2         GPIO_PIN_6
#define TIM_MOTOR_GPIO_AF					GPIO_AF2_TIM3

/* Private variables ---------------------------------------------------------*/
/* Timer handler declaration */
TIM_HandleTypeDef    TimHandle_Motor;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef              sPWMConfig;
/* Timer Break Configuration Structure declaration */
//TIM_BreakDeadTimeConfigTypeDef sBreakConfig;

void Motor_PWM_Init()
{
	GPIO_InitTypeDef   GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* TIMx Peripheral clock enable */
	TIM_MOTOR_CLK_ENABLE();

	/* Enable all GPIO Channels Clock requested */
	TIM_MOTOR_CHANNEL1_GPIO_PORT();
	TIM_MOTOR_CHANNEL2_GPIO_PORT();

	  /* Common configuration for all channels
	   * in push-pull, alternate function mode*/
	  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

	  /* GPIO TIM8_Channel1 configuration */
	  GPIO_InitStruct.Alternate = TIM_MOTOR_GPIO_AF;
	  GPIO_InitStruct.Pin = TIM_MOTOR_GPIO_PIN_CHANNEL1;
	  HAL_GPIO_Init(TIM_MOTOR_GPIO_PORT_CHANNEL1, &GPIO_InitStruct);

	  /* GPIO TIM8_Channel1N configuration */
	  GPIO_InitStruct.Alternate = TIM_MOTOR_GPIO_AF;
	  GPIO_InitStruct.Pin = TIM_MOTOR_GPIO_PIN_CHANNEL2;
	  HAL_GPIO_Init(TIM_MOTOR_GPIO_PORT_CHANNEL2, &GPIO_InitStruct);

	  /* Initialize TIM peripheral as follows:
	   * PWM_frequency = 180000000 / 18 / 500 = 20000 = 20kHz
	       + Prescaler = (18 - 1)
	       + Period = (500 - 1)  (to have an output frequency equal to 20 KHz)
	       + ClockDivision = 0
	       + Counter direction = Up
	  */
	  /* Select the Timer instance */
	  TimHandle_Motor.Instance = TIM_MOTOR;

	  TimHandle_Motor.Init.Prescaler         = PRESCALER;
	  TimHandle_Motor.Init.Period            = PERIOD;
	  TimHandle_Motor.Init.ClockDivision     = 0;
	  TimHandle_Motor.Init.CounterMode       = TIM_COUNTERMODE_CENTERALIGNED3;
	  if(HAL_TIM_PWM_Init(&TimHandle_Motor) != HAL_OK)
	  {
	    /* Initialization Error */
	    Error_Handler();
	  }

	  /*##-2- Configure the PWM channels #########################################*/
	  /* Common configuration for all channels */
	  sPWMConfig.OCMode       = TIM_OCMODE_PWM1;
	  sPWMConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
	  sPWMConfig.OCFastMode   = TIM_OCFAST_DISABLE;

	  /* Set the pulse value for channel 1 */
	  sPWMConfig.Pulse = START_DUTYCCLE1*DUTY_1percent; //
	  if(HAL_TIM_PWM_ConfigChannel(&TimHandle_Motor, &sPWMConfig, TIM_CHANNEL_1) != HAL_OK)
	  {
	    /* Configuration Error */
	    Error_Handler();
	  }

	  /* Set the pulse value for channel 2 */
	  sPWMConfig.Pulse = START_DUTYCCLE2*DUTY_1percent; //
	  if(HAL_TIM_PWM_ConfigChannel(&TimHandle_Motor, &sPWMConfig, TIM_CHANNEL_2) != HAL_OK)
	  {
	    /* Configuration Error */
	    Error_Handler();
	  }


	  /*##-3- Start PWM signals generation #######################################*/
	  /* Start channel 1 */
	  if(HAL_TIM_PWM_Start(&TimHandle_Motor, TIM_CHANNEL_1) != HAL_OK)
	  {
	    /* Starting Error */
	    Error_Handler();
	  }

	  /* Start channel 2 */
	  if(HAL_TIM_PWM_Start(&TimHandle_Motor, TIM_CHANNEL_2) != HAL_OK)
	  {
	    /* Starting Error */
	    Error_Handler();
	  }
}

/* -40 és 40 közötti értéket vár */
void SetSpeed(int8_t Speed)
{
	if(Speed<-45 || Speed>45)
	{
		MotorStop();
		Error_SendUart("Tartomanyon kivuli sebesseg ertek! \n\r");
	}

	else
	{
		/* jelenleg megengedett max sebesség */
		if(Speed<-MAX_SPEED)
			Speed = -MAX_SPEED;
		if(Speed>MAX_SPEED)
			Speed = MAX_SPEED;

			  /* Set the pulse value for channel 1 */
			  sPWMConfig.Pulse = (50-Speed)*DUTY_1percent; //
			  if(HAL_TIM_PWM_ConfigChannel(&TimHandle_Motor, &sPWMConfig, TIM_CHANNEL_1) != HAL_OK)
			  {
				/* Configuration Error */
				Error_Handler();
			  }

			  /* Set the pulse value for channel 2 */
			  sPWMConfig.Pulse = (50+Speed)*DUTY_1percent; //
			  if(HAL_TIM_PWM_ConfigChannel(&TimHandle_Motor, &sPWMConfig, TIM_CHANNEL_2) != HAL_OK)
			  {
				/* Configuration Error */
				Error_Handler();
			  }


			  /*##-3- Start PWM signals generation #######################################*/
			  /* Start channel 1 */
			  if(HAL_TIM_PWM_Start(&TimHandle_Motor, TIM_CHANNEL_1) != HAL_OK)
			  {
				/* Starting Error */
				Error_Handler();
			  }

			  /* Start channel 2 */
			  if(HAL_TIM_PWM_Start(&TimHandle_Motor, TIM_CHANNEL_2) != HAL_OK)
			  {
				/* Starting Error */
				Error_Handler();
			  }
	}
}

/* Motor szabadon fut, minden FET zárva */
void MotorStop()
{
	HAL_TIM_PWM_Stop(&TimHandle_Motor, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&TimHandle_Motor, TIM_CHANNEL_2);
}
