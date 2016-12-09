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

//t�vir�ny�t� t�sk�inek sz�r�se
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

	/* Perif�ri�k inicializ�l�sa */
	Periph_Init();

	/* CAN */
	if(CAN_ReceiveIT() != HAL_OK)
		Error_Handler();

	/*UART */
	UART1_RecvStringNonBlocking();

	for(;;)
	{
		//HAL_Delay(10);

		//Vonalk�vet�s: PD szab�lyz�
		Do_PositionControl();

		//teszt: vonalt�pus
		GetLineType();

		//teszt: sebess�g �ll�t�s vonalt�pus alapj�n
		Do_SpeedControl_FixSpeed();

		//v�szle�ll�t�s t�vir�ny�t�val
		if(uwDutyCycle<70)
			StateQ1 = Stop;

		//Sebess�g: t�vir�ny�t�
		//SetSpeed_RemoteControl();

	/*	//Teszt: korm�ny k�veti a vonalat
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
		//Motorvez�rl� teszt + t�vir�ny�t�
		//uwDutyCycle: 57-140 k�z�tt.
	/*	static int8_t value;
		if(uwDutyCycle < 150 && uwDutyCycle > 50)
			value = (uwDutyCycle-97)>>1;
		else
			value = 0;
		if(value<4 && value>-4)
			value=0;

		//t�vir�ny�t� zaj sz�r�s
		if(value==0)	//ha nulla a sebess�g, el�lr�l kezdj�k az id�m�r�st
			temp=0;
		else
			temp++;
		//csak akkor k�ldj�n sebess�gjelet, ha egy ideje(temp �rt�ke) m�r veszi a jelet a t�vir�ny�t�t�l,
		//de a nulla �rt�ket mindig k�ldeni kell neki, hogy ne az el�z� �rt�k ragadjon be
		if(temp>5 || value==0)
		{
			SetSpeed(value);
			temp=0;
		}
		*/
	}//for
}//main
