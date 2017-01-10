/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @author  Ac6
  * @version V1.0
  * @date    02-Feb-2015
  * @brief   Default Interrupt Service Routines.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#ifdef USE_RTOS_SYSTICK
#include <cmsis_os.h>
#endif
#include "stm32f4xx_it.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            	  	    Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles SysTick Handler, but only if no RTOS defines it.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
#ifdef USE_RTOS_SYSTICK
	osSystickHandler();
#endif
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f446xx.s).                                             */
/******************************************************************************/

/**
  * @brief  This function handles User Button 0 Interrupt request
  */
void EXTI0_IRQHandler(void){
	/* User Button 0 */
	HAL_GPIO_EXTI_IRQHandler(USER_BUTTON0_PIN);
}

/**
  * @brief  This function handles User Button 1 Interrupt request
  */
void EXTI1_IRQHandler(void){
	/* User Button 1 */
	HAL_GPIO_EXTI_IRQHandler(USER_BUTTON1_PIN);
}

/**
  * @brief  This function handles Nucleo User Button Interrupt request
  */
void EXTI15_10_IRQHandler(void){
	/* User Button */
	HAL_GPIO_EXTI_IRQHandler(USER_BUTTON_PIN);
}

/**
* @brief  This function handles ADC1 DMA interrupt request.
*/
void DMA2_Stream0_IRQHandler(void){
	HAL_DMA_IRQHandler(Adc1Handle.DMA_Handle);
}

/**
* @brief This function handles USART2 (Cable) global interrupt.
*/
void USART2_IRQHandler(void){
	HAL_UART_IRQHandler(&UartHandle_Cable);
}

/**
* @brief This function handles USART1 (Bluetooth) global interrupt.
*/
void USART1_IRQHandler(void){
	HAL_UART_IRQHandler(&UartHandle_Bluetooth);
}

/**
* @brief This function handles UART5 (Radio) global interrupt.
*/
void UART5_IRQHandler(void){
	HAL_UART_IRQHandler(&UartHandle_Radio);
}

/**
* @brief  This function handles CAN1 TX interrupt request.
*/
void CAN1_TX_IRQHandler(void){
	HAL_CAN_IRQHandler(&CanHandle);
}

/**
* @brief  This function handles CAN1 RX0 interrupt request.
*/
void CAN1_RX0_IRQHandler(void){
	HAL_CAN_IRQHandler(&CanHandle);
}

/**
* @brief This function handles Encoder Callback Timer interrupt request
*/
void TIM1_BRK_TIM9_IRQHandler(void){
	HAL_TIM_IRQHandler(&Encoder_Callback_Handle);
}

void TIM_REMOTE_IRQHandler(void){
	HAL_TIM_IRQHandler(&TimHandle_Remote);
}

void TIM_SZABALYZO_IRQHandler(void){
	HAL_TIM_IRQHandler(&TimHandle_Szabalyzo);
}
void TIM8_UP_TIM13_IRQHandler(){
	HAL_TIM_IRQHandler(&Adc1_Tim_Handle);
}
