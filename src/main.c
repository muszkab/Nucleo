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
	UART_RecvStringNonBlocking(&UartHandle_Cable);

	Led_On(Green);
	Led_On(Blue);
	Led_On(Yellow);

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

		//teszt: UART és RobotDiagnostic
	/*	static int temp=0;
		UART_SendNumberBlocking(temp++, &UartHandle_Bluetooth);
	*/

		//teszt: IMU
		static uint8_t who=3;

		GYRO_Driver->Get_WhoAmI(&GYRO_SensorHandle, &who);
		if( !ACCELERO_Driver->Check_WhoAmI(&ACCELERO_SensorHandle) )
			UART_SendStringBlocking("Siker!", &UartHandle_Cable);
		//LSM6DS3_G_Drv.Get_Axes_Status
		UART_SendNumberBlocking(who, &UartHandle_Cable);
		HAL_Delay(1000);
	}//for
}//main
