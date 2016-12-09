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

//távirányító tüskéinek szûrése
int16_t temp = 0;

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

	/* CAN */
	if(CAN_ReceiveIT() != HAL_OK)
		Error_Handler();

	/*UART */
	UART1_RecvStringNonBlocking();

	for(;;)
	{
		//HAL_Delay(10);

		//Vonalkövetés: PD szabályzó
		Do_PositionControl();

		//teszt: vonaltípus
		GetLineType();

		//teszt: sebesség állítás vonaltípus alapján
		Do_SpeedControl_FixSpeed();

		//vészleállítás távirányítóval
		if(uwDutyCycle<70)
			StateQ1 = Stop;

		//Sebesség: távirányító
		//SetSpeed_RemoteControl();

	/*	//Teszt: kormány követi a vonalat
		if(FrontSensor_Data[0] != 0)
		{
			temp = (FrontSensor_Data[0]-127)*3;
			if(temp>127)
				temp=127;
			if(temp<-127)
				temp=-127;
			ServoPos=-temp+127;
		}

		UART1_SendNumberBlocking(ServoPos);
		UART1_SendStringBlocking("\n\r");

		UART1_SendNumberBlocking(FrontSensor_Data[0]);
		UART1_SendStringBlocking("\n\r");

		SetServoPWMPulse(ServoPos);
	*/
		//Motorvezérlõ teszt + távirányító
		//uwDutyCycle: 57-140 között.
	/*	static int8_t value;
		if(uwDutyCycle < 150 && uwDutyCycle > 50)
			value = (uwDutyCycle-97)>>1;
		else
			value = 0;
		if(value<4 && value>-4)
			value=0;

		//távirányító zaj szûrés
		if(value==0)	//ha nulla a sebesség, elölrõl kezdjük az idõmérést
			temp=0;
		else
			temp++;
		//csak akkor küldjön sebességjelet, ha egy ideje(temp értéke) már veszi a jelet a távirányítótól,
		//de a nulla értéket mindig küldeni kell neki, hogy ne az elõzõ érték ragadjon be
		if(temp>5 || value==0)
		{
			SetSpeed(value);
			temp=0;
		}
		*/
	}//for
}//main
