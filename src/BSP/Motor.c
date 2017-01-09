/*
 * Motor.c
 *
 *  Created on: 2016. nov. 22.
 *      Author: Muszka
 */

/* Includes ------------------------------------------------------------------*/
#include "Motor.h"

/*****************************************************/
/**************** SAJÁT MOTORVEZÉRLÕ *****************/
/*****************************************************/
#define MAX_SPEED		35	//maximum: 45
#define START_DUTYCCLE1 50
#define START_DUTYCCLE2 50

/* TIM3 - PWM */
#define MAX_CLOCK		90				/* TIM3 max clock: 90MHz */
#define PRESCALER		(4-1)			/* TIM_freq=MAX_CLOCK*10^6/(Prescaler+1) */ //TIM3_freq=90MHZ/4 =22.5MHz
#define PERIOD			(500-1)			/* PWM_frequency=TIM_freq/(Period+1)=22.5MHZ/500/2=22.5kHZ
										 * 500/2: center aligned pwm miatt máshogy van a PERIOD érték*/
#define DUTY_1percent	(PERIOD+1)/100	/* 1%-os kitöltési tényezõ */

/* Private variables ---------------------------------------------------------*/
/* Timer handler declaration */
TIM_HandleTypeDef    TimHandle_Motor;
/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef              sPWMConfig;
/* Timer Break Configuration Structure declaration */
//TIM_BreakDeadTimeConfigTypeDef sBreakConfig;

/*****************************************************/
/**************** GYÁRI MOTORVEZÉRLÕ *****************/
/*****************************************************/
//100% - 20ms (f=50Hz, T=20ms)
#define	FACTORY_PULSE_UNIT		(PERIOD+1)/200		//0.1 ms, jelenleg nem használjuk
#define FACTORY_PULSE_MIN		1000//1260			//1 ms
#define FACTORY_PULSE_MAX		2000//1680			//2 ms
#define FACTORY_PULSE_CENTER	1500				//1.5 ms

/* TIM8 channel4 - PC9 - PWM */
#define FACTORY_MAX_CLOCK		180				/* TIM5 max clock: 180MHz */
#define FACTORY_PRESCALER		(MAX_CLOCK-1)	/* TIM_freq=MAX_CLOCK*10^6/(Prescaler+1) */ //TIM10_freq=180MHZ/180 =1MHz
#define FACTORY_PERIOD			(20000-1)		/* PWM_frequency=TIM_freq/(Period+1)=1MHZ/20000=10^6HZ/20000 =50HZ */

/* Private variables ---------------------------------------------------------*/
/* Timer handler declaration */
TIM_HandleTypeDef    TimHandle_Motor_Factory;
/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef sPWMConfigFactory;

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

void Motor_PWM_Init_Factory()
{
	GPIO_InitTypeDef   GPIO_InitStruct;
	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* TIMx Peripheral clock enable */
	TIM_MOTOR_FACTORY_CLK_ENABLE();

	/* Enable all GPIO Channels Clock requested */
	TIM_MOTOR_FACTORY_CHANNEL_GPIO_PORT();

	/* Configure PA2 in output, push-pull, alternate function mode*/
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

	GPIO_InitStruct.Alternate = TIM_MOTOR_FACTORY_GPIO_AF;
	GPIO_InitStruct.Pin = TIM_MOTOR_FACTORY_GPIO_PIN_CHANNEL;
	HAL_GPIO_Init(TIM_MOTOR_FACTORY_GPIO_PORT_CHANNEL, &GPIO_InitStruct);

	/*##-2- Configure the TIM peripheral #######################################*/
	/* f=50Hz =180MHz/180/20000 */
	/* Initialize TIMx peripheral as follows:
	   + Prescaler = 180 - 1
	   + Period = 20000 - 1
	   + ClockDivision = 1
	   + Counter direction = Up
	*/
	TimHandle_Motor_Factory.Instance 			   = TIM_MOTOR_FACTORY;
	TimHandle_Motor_Factory.Init.Prescaler         = FACTORY_PRESCALER;
	TimHandle_Motor_Factory.Init.Period            = FACTORY_PERIOD;
	TimHandle_Motor_Factory.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
	TimHandle_Motor_Factory.Init.CounterMode       = TIM_COUNTERMODE_UP;
	if (HAL_TIM_PWM_Init(&TimHandle_Motor_Factory) != HAL_OK){
		/* Initialization Error */
		Error_Handler();
	}

	/*##-3- Configure the PWM channels #########################################*/
	/* Common configuration for all channels */
	sPWMConfigFactory.OCMode       = TIM_OCMODE_PWM1;
	sPWMConfigFactory.OCPolarity   = TIM_OCPOLARITY_HIGH;
	sPWMConfigFactory.OCFastMode   = TIM_OCFAST_DISABLE;
	sPWMConfigFactory.Pulse		   = FACTORY_PULSE_CENTER;
	if (HAL_TIM_PWM_ConfigChannel(&TimHandle_Motor_Factory, &sPWMConfigFactory, TIM_MOTOR_FACTORY_CHANNEL) != HAL_OK){
		/* Configuration Error */
		Error_Handler();
	}

	/*##-4- Start PWM signals generation #######################################*/
	/* Start channel 1 */
	if (HAL_TIM_PWM_Start(&TimHandle_Motor_Factory, TIM_MOTOR_FACTORY_CHANNEL) != HAL_OK){
		/* PWM Generation Error */
		Error_Handler();
	}
}

/* -45 és 45 közötti értéket vár */
void SetSpeed(int8_t Speed)
{
	//hibás bemeneti paraméter ellen, hibás mûködés esetén a motor megáll
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

		TimHandle_Motor.Instance->CCR1=(50-Speed)*DUTY_1percent;
		TimHandle_Motor.Instance->CCR2=(50+Speed)*DUTY_1percent;
	}
}

/* Speed=0:    center
 * Speed=127:  right
 * Speed=-128: left*/
void SetSpeedFactory(int8_t Speed){
	TimHandle_Motor_Factory.Instance->CCR4=FACTORY_PULSE_CENTER+
			(Speed)*(FACTORY_PULSE_MAX-FACTORY_PULSE_MIN)/255;
}

/* Motor megáll, felsõ FETek nem vezetnek, alsó FETek vezetnek */
void MotorStop()
{
	HAL_TIM_PWM_Stop(&TimHandle_Motor, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&TimHandle_Motor, TIM_CHANNEL_2);
}
