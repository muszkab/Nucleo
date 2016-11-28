/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "stm32f4xx_hal_msp.h"


int main(void)
{
	/* STM32F4xx HAL library initialization:
	   - Configure the Flash prefetch
	   - Systick timer is configured by default as source of time base, but user
		 can eventually implement his proper time base source (a general purpose
		 timer for example or other time source), keeping in mind that Time base
		 duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
		 handled in milliseconds basis.
	   - Set NVIC Group Priority to 4
	   - Low Level Initialization
	 */
	HAL_Init();
	/* Set NVIC Group Priority to 3 */
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_3);

	/* Configure the system clock to 180 MHz */
	SystemClock_Config();

	/* Perifériák inicializálása */
	Periph_Init();

	/*Test*/
	/* CAN */
	/*if(CAN_TransmitIT(message,length) != HAL_OK)
		Error_Handler();*/

	if(CAN_ReceiveIT() != HAL_OK)
		Error_Handler();


//	/*UART test */
//	UART_Send(CanHandle.pRxMsg->Data,CanHandle.pRxMsg->DLC);
//	UART_NewLine();
//	UART_SendString("Nucleo");
//	UART_SendString("Orsi");
//	UART_SendInt(1992);
//	UART_NewLine();
//	UART1_SendStringNonBlocking("Balazs \n\r");
	UART1_RecvStringNonBlocking();

//	/* Servo PWM test */

/*	HAL_Delay(3000);
	TIM10->CCR1=1800;
	HAL_Delay(3000);
	TIM10->CCR1=1200;
	HAL_Delay(3000);
	TIM10->CCR1=1460;
	SetServoPWMPulse(255);
	HAL_Delay(3000);
	SetServoPWMPulse(20);
	HAL_Delay(3000);
	SetServoPWMPulse(120);
*/
	int16_t temp = 0;
	for(;;)
	{
		HAL_Delay(10);

		//Teszt: kormány követi a vonalat
		temp = (FrontSensor_Data[0]-127)*3;
		if(temp>127)
			temp=127;
		if(temp<-127)
			temp=-127;
		ServoPos=-temp+127;
		if(FrontSensor_Data[0] == 0)
			ServoPos = 127;
		/*
		if(ServoPos>200)
			ServoPos = 180;
		if(ServoPos<70)
			ServoPos = 70;
			*/

		UART1_SendNumberBlocking(ServoPos);
		UART1_SendStringBlocking("\n\r");

		UART1_SendNumberBlocking(FrontSensor_Data[0]);
		UART1_SendStringBlocking("\n\r");

		SetServoPWMPulse(ServoPos);

		//Motorvezérlõ teszt + távirányító
		//uwDutyCycle: 57-140 között.
		static int8_t value;
		if(uwDutyCycle < 150 && uwDutyCycle > 50)
			value = (uwDutyCycle-97)>>1;
		else
			value = 0;
		if(value<4 && value>-4)
			value=0;
		SetSpeed(value);
	}
}
