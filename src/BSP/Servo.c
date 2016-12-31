/*
 * Servo.c
 *
 *  Created on: 2016. okt. 22.
 *      Author: Muszka
 */

/* Includes ------------------------------------------------------------------*/
#include "Servo.h"


/**********************************************/
/*********** Servo - Steering Wheel ***********/
/**********************************************/
//TODO A CLK és GPIO definiálás bõven elfér a headerben, ezek nem változó paraméterek
//100% - 20ms (f=50Hz, T=20ms)
#define	PULSE_UNIT		(PERIOD+1)/200		//0.1ms
#define PULSE_MIN		1250//1260			//1.24ms left
#define PULSE_MAX		1690//1680			//1.68ms right
#define PULSE_CENTER	1460				//1.46ms

/* TIM10 channel1 - PC8 - PWM */
#define MAX_CLOCK		180				/* TIM10 max clock: 180MHz */
#define PRESCALER		(MAX_CLOCK-1)	/* TIM_freq=MAX_CLOCK*10^6/(Prescaler+1) */ //TIM10_freq=180MHZ/180 =1MHz
#define PERIOD			(20000-1)		/* PWM_frequency=TIM_freq/(Period+1)=1MHZ/20000=10^6HZ/20000 =50HZ */

/**********************************************/
/*************** Servo - Sharp ****************/
/**********************************************/
//TODO frekvenciát le kell ellenõrizni
/* TIM4 channel2 - PB7 - PWM */
#define SERVO_SHARP_MAX_CLOCK	90						  /* TIM4 max clock: 90MHz */
#define SERVO_SHARP_PRESCALER	(SERVO_SHARP_MAX_CLOCK-1) /* TIM_freq=MAX_CLOCK*10^6/(Prescaler+1) */ //TIM10_freq=180MHZ/180 =1MHz
#define SERVO_SHARP_PERIOD		(20000-1)				  /* PWM_frequency=TIM_freq/(Period+1)=1MHZ/20000=10^6HZ/20000 =50HZ */

//100% - 20ms (f=50Hz, T=20ms)
#define	SERVO_SHARP_PULSE_UNIT		(SERVO_SHARP_PERIOD+1)/200		//0.1ms
#define SERVO_SHARP_PULSE_MIN		1250//1260			//1.24ms left
#define SERVO_SHARP_PULSE_MAX		1690//1680			//1.68ms right
#define SERVO_SHARP_PULSE_CENTER	1460				//1.46ms

/* Private variables ---------------------------------------------------------*/
/* Timer handler declaration */
TIM_HandleTypeDef    TimHandle_Servo;
TIM_HandleTypeDef    TimHandle_Servo_Sharp;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef sConfig_Servo;

/*
 * Steering Wheel Servo init -> TIM10
 */
void Servo_TIM10_PWM_Init()
{
	GPIO_InitTypeDef   GPIO_InitStruct;
	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* TIMx Peripheral clock enable */
	TIMx_CLK_ENABLE();

	/* Enable all GPIO Channels Clock requested */
	TIMx_CHANNEL_GPIO_PORT();

	/* Configure PB8 in output, push-pull, alternate function mode*/
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

	GPIO_InitStruct.Alternate = TIMx_GPIO_AF_CHANNEL1;
	GPIO_InitStruct.Pin = TIMx_GPIO_PIN_CHANNEL1;
	HAL_GPIO_Init(TIMx_GPIO_PORT_CHANNEL1, &GPIO_InitStruct);

	  /*##-1- Configure the TIM peripheral #######################################*/
	  /* f=50Hz =180MHz/180/20000 */
	  /* Initialize TIMx peripheral as follows:
		   + Prescaler = 180 - 1
		   + Period = 20000 - 1
		   + ClockDivision = 1
		   + Counter direction = Up
	  */
	  TimHandle_Servo.Instance = TIMx;

	  TimHandle_Servo.Init.Prescaler         = PRESCALER;
	  TimHandle_Servo.Init.Period            = PERIOD;
	  TimHandle_Servo.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
	  TimHandle_Servo.Init.CounterMode       = TIM_COUNTERMODE_UP;
	  if (HAL_TIM_PWM_Init(&TimHandle_Servo) != HAL_OK)
	  {
		/* Initialization Error */
		Error_Handler();
	  }

	  /*##-2- Configure the PWM channels #########################################*/
	  /* Common configuration for all channels */
	  sConfig_Servo.OCMode       = TIM_OCMODE_PWM1;
	  sConfig_Servo.OCPolarity   = TIM_OCPOLARITY_HIGH;
	  sConfig_Servo.OCFastMode   = TIM_OCFAST_DISABLE;
	  sConfig_Servo.Pulse		 = PULSE_CENTER;
	  if (HAL_TIM_PWM_ConfigChannel(&TimHandle_Servo, &sConfig_Servo, TIM_CHANNEL_1) != HAL_OK)
	  {
		/* Configuration Error */
		Error_Handler();
	  }

	  /*##-3- Start PWM signals generation #######################################*/
	  /* Start channel 1 */
	  if (HAL_TIM_PWM_Start(&TimHandle_Servo, TIM_CHANNEL_1) != HAL_OK)
	  {
		/* PWM Generation Error */
		Error_Handler();
	  }
}

/*
 * Sharp Servo init -> TIM11
 */
void Servo_Sharp_TIM4_PWM_Init(){
	GPIO_InitTypeDef   GPIO_InitStruct;
	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* TIM4 Peripheral clock enable */
	SERVO_SHARP_TIM_CLK_ENABLE();

	/* Enable all GPIO Channels Clock requested */
	TIM_SERVO_SHARP_CHANNEL_GPIO_PORT();

	/* Configure PB7 in output, push-pull, alternate function mode*/
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = TIM_SERVO_SHARP_GPIO_AF_CHANNEL2;
	GPIO_InitStruct.Pin = TIM_SERVO_SHARP_GPIO_PIN_CHANNEL2;
	HAL_GPIO_Init(TIM_SERVO_SHARP_GPIO_PORT_CHANNEL2, &GPIO_InitStruct);

	/*##-1- Configure the TIM peripheral #######################################*/
	/* f=50Hz =180MHz/180/20000 */
	/* Initialize TIM4 peripheral as follows:
	   + Prescaler = 180 - 1
	   + Period = 20000 - 1
	   + ClockDivision = 1
	   + Counter direction = Up
	*/
	TimHandle_Servo_Sharp.Instance 				 = SERVO_SHARP_TIM;
	TimHandle_Servo_Sharp.Init.Prescaler         = SERVO_SHARP_PRESCALER;
	TimHandle_Servo_Sharp.Init.Period            = SERVO_SHARP_PERIOD;
	TimHandle_Servo_Sharp.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
	TimHandle_Servo_Sharp.Init.CounterMode       = TIM_COUNTERMODE_UP;
	if (HAL_TIM_PWM_Init(&TimHandle_Servo_Sharp) != HAL_OK){
		/* Initialization Error */
		Error_Handler();
	}

	/*##-2- Configure the PWM channels #########################################*/
	/* Common configuration for all channels */
	sConfig_Servo.OCMode       = TIM_OCMODE_PWM1;
	sConfig_Servo.OCPolarity   = TIM_OCPOLARITY_HIGH;
	sConfig_Servo.OCFastMode   = TIM_OCFAST_DISABLE;
	sConfig_Servo.Pulse		   = SERVO_SHARP_PULSE_CENTER;
	if (HAL_TIM_PWM_ConfigChannel(&TimHandle_Servo_Sharp, &sConfig_Servo, TIM_CHANNEL_2) != HAL_OK){
		/* Configuration Error */
		Error_Handler();
	}

	/*##-3- Start PWM signals generation #######################################*/
	/* Start channel 1 */
	if (HAL_TIM_PWM_Start(&TimHandle_Servo_Sharp, TIM_CHANNEL_2) != HAL_OK){
		/* PWM Generation Error */
		Error_Handler();
	}
}

/* ServoPos=0: 	  center
 * ServoPos=127:  right
 * ServoPos=-128: left*/
void SetServoPWMPulse(const int8_t ServoPos){
	if(ServoPos>=-128 && ServoPos<=127)
	{
		TimHandle_Servo.Instance->CCR1=PULSE_CENTER+(ServoPos)*(PULSE_MAX-PULSE_MIN)/255;
	}
}

/* ServoPos=0: 	  center
 * ServoPos=127:  right
 * ServoPos=-128: left*/
void SetServoSharpPWMPulse(const int8_t ServoPos){
	if(ServoPos>=-128 && ServoPos<=127){
		TimHandle_Servo_Sharp.Instance->CCR2=SERVO_SHARP_PULSE_CENTER+(ServoPos)
				*(SERVO_SHARP_PULSE_MAX-SERVO_SHARP_PULSE_MIN)/255;
	}
}
