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
#include "stm32f4xx_hal_msp.h"

#include "Encoder.h"

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

/**
  * @brief  This function handles ADC interrupt request.
  * @param  None
  * @retval None
  */
void ADC_IRQHandler(void)
{
  HAL_ADC_IRQHandler(&AdcHandle);
}

void EXTI15_10_IRQHandler(void)
{
	/* User Button*/
	HAL_GPIO_EXTI_IRQHandler(USER_BUTTON_PIN);
}

void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&UartHandle);
}

void CAN1_TX_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&CanHandle);
}

void CAN1_RX0_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&CanHandle);
}

void TIM_HalfPeriod_IRQHandler(void)
{
	/* Segéd timer, f=2MHz */
	HAL_TIM_IRQHandler(&TimHandle_HalfPeriod);
}

void TIM_Encoder_IRQHandler(void)
{
	/* Enkóder A csatorna élváltás */
	HAL_TIM_IRQHandler(&TimHandle_Encoder);
}

void TIM_REMOTE_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TimHandle_Remote);
}
