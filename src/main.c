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
	//HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_3);

	/* Configure the system clock to 180 MHz */
	SystemClock_Config();

	/* Perif�ri�k inicializ�l�sa */
	Periph_Init();

	/* CAN */
	if(CAN_ReceiveIT() != HAL_OK)
		Error_Handler();

	/*UART */
	UART_RecvStringNonBlocking(&UartHandle_Cable);


	//teszt: IMU
	static uint8_t who=3;
	//static SensorAxes_t SensorAxes;

	GYRO_Driver->Get_WhoAmI(&GYRO_SensorHandle, &who);

	//if( GYRO_Driver->Init(&GYRO_SensorHandle) != COMPONENT_OK )
		//Error_SendUart("GYRO Init error!");
	//if( GYRO_Driver->Sensor_Enable(&GYRO_SensorHandle) != COMPONENT_OK )
		//Error_SendUart("GYRO Enable error!");

	//teszt v�ltoz�k
	Messagetype message[2];
	message[0].Name="Value:P_Corner";
	message[0].Data=3.456;

	message[1].Name="Value:P_Straight";
	message[1].Data=150;

	message[2].Name="Value:Time";
	message[2].Data=0;

	Messagetype messageGraph[1];
	messageGraph[0].Name="Graph:Linepos";
	messageGraph[0].Data=1;

	SendDebugMessage_Raw("Text:Orsi",0);
	SendDebugMessage_Text("Itt lehet szoveget kuldeni.");

	for(;;)
	{
		//HAL_Delay(10);

		//Vonalk�vet�s: PD szab�lyz�
		Do_PositionControl();

		//Vonalt�pus meghat�roz�s
		Do_GetLineType();

		//Sebess�g: fix �rt�k vonalt�pus alapj�n, Q1
		Do_SpeedControl_FixSpeed();

		//Sebess�g: t�vir�ny�t�
		//SetSpeed_RemoteControl();

		//�zenetk�ld�s Bluetooth-on
		Do_Send_ValueMessageArray();

		//v�szle�ll�t�s t�vir�ny�t�val
		Is_StopCommand();

		if(uwDutyCycle<70)
			StateQ1 = Stop;


		//teszt: IMU
	/*	GYRO_Driver->Get_Axes(&GYRO_SensorHandle, &SensorAxes);
		//LSM6DS3_G_Drv.Get_Axes_Status
		UART_SendNumberBlocking(SensorAxes.AXIS_X, &UartHandle_Cable);
		UART_SendStringBlocking("\n\r", &UartHandle_Cable);
		HAL_Delay(100);
	*/

		//teszt: Debugszoftver
		SendDebugMessage_Array(message, 3);
		message[2].Data++;
		message[1].Data--;

		SendDebugMessage_Array(messageGraph, 1);
		if(messageGraph[0].Data>-50)
			messageGraph[0].Data--;

		//SendDebugMessage_Raw("Text:Kiraly",0);

		HAL_Delay(200);


	}//for
}//main
