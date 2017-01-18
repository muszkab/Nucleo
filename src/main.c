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

uint8_t GYR_Who=0;
SensorAxes_t GYR_AXIS;
SensorAxes_t GYR_AXIS_CAL;
float GYR_Sensitivity;
float GYR_FullScale;

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

	/* IMU */
	ACCELERO_Driver->Get_WhoAmI(&ACCELERO_SensorHandle,&ACC_Who);
	if( ACCELERO_Driver->Init(&ACCELERO_SensorHandle) != COMPONENT_OK )
		Error_SendUart("ACC Init error!");
	if( ACCELERO_Driver->Sensor_Enable(&ACCELERO_SensorHandle) != COMPONENT_OK )
		Error_SendUart("GYRO Enable error!");
	if( GYRO_Driver->Init(&GYRO_SensorHandle) != COMPONENT_OK )
		Error_SendUart("GYRO Init error!");
	if( GYRO_Driver->Sensor_Enable(&GYRO_SensorHandle) != COMPONENT_OK )
		Error_SendUart("GYRO Enable error!");

	//teszt változók
	//Messagetype messageGraph[1];
	//messageGraph[0].Name="Graph:Linepos";
	//messageGraph[0].Data=1;

	SendDebugMessage_Text("A");
	SendDebugMessage_Text("Itt lehet szoveget kuldeni.");

	Calibrate_Gyro();
	/*##-3- Start timer #######################################*/
	if (HAL_TIM_Base_Start_IT(&TimHandle_Gyro) != HAL_OK)
	{
		Error_Handler();
	}

	for(;;)
	{
		//ACCELERO_Driver->Get_WhoAmI(&ACCELERO_SensorHandle,&ACC_Who);
		//ACCELERO_Driver->Get_Axes(&ACCELERO_SensorHandle,&ACC_AXIS);
		//ACCELERO_Driver->Get_Sensitivity(&ACCELERO_SensorHandle,&ACC_Sensitivity);
		//ACCELERO_Driver->Get_FS(&ACCELERO_SensorHandle,&ACC_FullScale);

		//GYRO_Driver->Get_WhoAmI(&GYRO_SensorHandle,&GYR_Who);
		//GYRO_Driver->Get_Axes(&GYRO_SensorHandle,&GYR_AXIS);
		//GYR_AXIS.AXIS_X -= GYR_AXIS_CAL.AXIS_X;
		//GYR_AXIS.AXIS_Y -= GYR_AXIS_CAL.AXIS_Y;
		//GYR_AXIS.AXIS_Z -= GYR_AXIS_CAL.AXIS_Z;
		//GYRO_Driver->Get_Sensitivity(&GYRO_SensorHandle,&GYR_Sensitivity);
		//GYRO_Driver->Get_FS(&GYRO_SensorHandle,&GYR_FullScale);

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
