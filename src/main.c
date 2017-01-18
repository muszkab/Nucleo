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

//teszt: IMU
uint8_t ACC_Who=0;
SensorAxes_t ACC_AXIS;
float ACC_Sensitivity;
float ACC_FullScale;

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

	/* UART */
	UART_RecvStringNonBlocking(&UartHandle_Cable);

	//teszt változók
	//Messagetype messageGraph[1];
	//messageGraph[0].Name="Graph:Linepos";
	//messageGraph[0].Data=1;

	SendDebugMessage_Text("A");
	SendDebugMessage_Text("Itt lehet szoveget kuldeni.");

	Calibrate_Gyro();
	IMU_Degre_Calc_Start();

	for(;;)
	{
		//ACCELERO_Driver->Get_WhoAmI(&ACCELERO_SensorHandle,&ACC_Who);
		//ACCELERO_Driver->Get_Axes(&ACCELERO_SensorHandle,&ACC_AXIS);
		//ACCELERO_Driver->Get_Sensitivity(&ACCELERO_SensorHandle,&ACC_Sensitivity);
		//ACCELERO_Driver->Get_FS(&ACCELERO_SensorHandle,&ACC_FullScale);


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

		//Sebesség: fix érték vonaltípus alapján, Q1
		//Do_SpeedControl_FixSpeed();

		//Sebesség: távirányító
		//SetSpeed_RemoteControl();

		//SendDebugMessage_Array(messageGraph, 1);
		//if(messageGraph[0].Data>-50)
			//messageGraph[0].Data--;
	}//for
}//main
