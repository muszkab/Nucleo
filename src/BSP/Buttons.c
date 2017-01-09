/*
 * Buttons.c
 *
 *  Created on: 2016. dec. 24.
 *      Author: szabgyul92
 */

/* Includes ------------------------------------------------------------------*/
#include "Buttons.h"

/*User Button*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == USER_BUTTON_PIN)
	{
		MotorIdentification();
		//StateQ1 = CornerOut;
	}
	if(GPIO_Pin == USER_BUTTON0_PIN){
		LED_Toggle(Yellow);
	}
	if(GPIO_Pin == USER_BUTTON1_PIN){
		LED_Toggle(Blue);
	}
}

GPIO_TypeDef* BUTTONs_PORT[2] = {USER_BUTTON0_GPIO_PORT, USER_BUTTON1_GPIO_PORT};
const uint16_t BUTTONs_PIN[2] = {USER_BUTTON0_PIN, USER_BUTTON1_PIN};

/**
  * @brief  Configures Buttons GPIO and EXTI Line.
  * @param  ButtonMode: Specifies Button mode.
  *   This parameter can be one of following parameters:
  *     @arg BUTTON_MODE_GPIO: Button will be used as simple IO
  *     @arg BUTTON_MODE_EXTI: Button will be connected to EXTI line with interrupt
  *                            generation capability
  */
void Buttons_Init(ButtonMode_TypeDef ButtonMode){
	/* Enable the BUTTONs Clock */
	USER_BUTTON0_GPIO_CLK_ENABLE();
	USER_BUTTON1_GPIO_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;

	if(ButtonMode == BUTTON_MODE_GPIO){
		/* Configure Button pin as input */
		GPIO_InitStruct.Pin = USER_BUTTON0_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLUP; //PULLUP kell a 2 gombnak
		GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
		HAL_GPIO_Init(USER_BUTTON0_GPIO_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = USER_BUTTON1_PIN;
		HAL_GPIO_Init(USER_BUTTON1_GPIO_PORT, &GPIO_InitStruct);
	}

	if(ButtonMode == BUTTON_MODE_EXTI){
		/* Configure Buttons pin as input with External interrupt */
		GPIO_InitStruct.Pin = USER_BUTTON0_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		GPIO_InitStruct.Pull = GPIO_PULLUP; //PULLUP kell a 2 gombnak
		GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
		HAL_GPIO_Init(USER_BUTTON0_GPIO_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = USER_BUTTON1_PIN;
		HAL_GPIO_Init(USER_BUTTON1_GPIO_PORT, &GPIO_InitStruct);

		//Regiszter szinten kell állítani! Nehéz volt rájönni :)
		SYSCFG->EXTICR[0]=0x0077;

		/* Enable and set Button EXTI Interrupt to the lowest priority */
		HAL_NVIC_SetPriority(USER_BUTTON0_EXTI_IRQn, 3, 0);
		HAL_NVIC_EnableIRQ(USER_BUTTON0_EXTI_IRQn);

		HAL_NVIC_SetPriority(USER_BUTTON1_EXTI_IRQn, 3, 0);
		HAL_NVIC_EnableIRQ(USER_BUTTON1_EXTI_IRQn);
	}
}

/**
  * @brief  Returns the selected Button state.
  * @param  Button: Specifies the Button to be checked.
  *   This parameter can be one of following parameters:
  *     @arg BUTTON_USER_0
  *     @arg BUTTON_USER_1
  * @retval The Button GPIO pin value.
  */
uint32_t Button_GetState(Button_Type Button){
	return HAL_GPIO_ReadPin(BUTTONs_PORT[Button], BUTTONs_PIN[Button]);
}
