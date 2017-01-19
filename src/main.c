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

	/* Perifériák inicializálása */
	Periph_Init();

	/* CAN */
	CAN_ReceiveIT();

	/*UART */
	UART_RecvStringNonBlocking(&UartHandle_Cable);

	SendDebugMessage_Text("A");
	SendDebugMessage_Text("Itt lehet szoveget kuldeni.");


	//teszt változók
	//Messagetype messageGraph[1];
	//messageGraph[0].Name="Graph:Linepos";
	//messageGraph[0].Data=1;

	for(;;)
	{
		//HAL_Delay(1000);

		//Ügyességi pálya
		Do_SkillTrack();

		//Üzenetküldés Bluetooth-on
		//Do_Send_ValueMessageArray();

		//Vonaltípus meghatározás
		//Do_LineType();

		//Faltípus meghatározás
		//Do_WallType();

		//vészleállítás távirányítóval
		//Is_StopCommand();

		//Vonalkövetés: PD szabályzó
		//Do_PositionControl();

		//Vonalkövetés: Állapotteres szabályzó
		//Do_PositionControl_AT();

		//Sebességszabályozás
		//MotorControlSetVelocityRef(0);
		//Do_MotorControl();

		//Sebesség: fix érték vonaltípus alapján, Q1
		//Do_SpeedControl_FixSpeed();

		//Sebesség: távirányító
		//SetSpeed_RemoteControl();

		//SendDebugMessage_Array(messageGraph, 1);
		//if(messageGraph[0].Data>-50)
			//messageGraph[0].Data--;
	}//for
}//main
