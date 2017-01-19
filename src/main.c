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
	//HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_3);

	/* Configure the system clock to 180 MHz */
	SystemClock_Config();

	/* Perif�ri�k inicializ�l�sa */
	Periph_Init();

	/* CAN */
	CAN_ReceiveIT();

	/*UART */
	UART_RecvStringNonBlocking(&UartHandle_Cable);

	SendDebugMessage_Text("A");
	SendDebugMessage_Text("Itt lehet szoveget kuldeni.");


	//teszt v�ltoz�k
	//Messagetype messageGraph[1];
	//messageGraph[0].Name="Graph:Linepos";
	//messageGraph[0].Data=1;

	for(;;)
	{
		//HAL_Delay(1000);

		//�gyess�gi p�lya
		Do_SkillTrack();

		//�zenetk�ld�s Bluetooth-on
		//Do_Send_ValueMessageArray();

		//Vonalt�pus meghat�roz�s
		//Do_LineType();

		//Falt�pus meghat�roz�s
		//Do_WallType();

		//v�szle�ll�t�s t�vir�ny�t�val
		//Is_StopCommand();

		//Vonalk�vet�s: PD szab�lyz�
		//Do_PositionControl();

		//Vonalk�vet�s: �llapotteres szab�lyz�
		//Do_PositionControl_AT();

		//Sebess�gszab�lyoz�s
		//MotorControlSetVelocityRef(0);
		Do_MotorControl();

		//Sebess�g: fix �rt�k vonalt�pus alapj�n, Q1
		//Do_SpeedControl_FixSpeed();

		//Sebess�g: t�vir�ny�t�
		//SetSpeed_RemoteControl();

		//SendDebugMessage_Array(messageGraph, 1);
		//if(messageGraph[0].Data>-50)
			//messageGraph[0].Data--;
	}//for
}//main
