/*
 * Buttons.h
 *
 *  Created on: 2016. dec. 24.
 *      Author: szabgyul92
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BSP_BUTTONS_H_
#define BSP_BUTTONS_H_

/* Includes ------------------------------------------------------------------*/
#include "BSP.h"

/* Definition for Buttons clock resources */
#define USER_BUTTON0_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOH_CLK_ENABLE()
#define USER_BUTTON1_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOH_CLK_ENABLE()

/* Definition for Buttons Pins */
/* Button 0 - PH0*/
#define USER_BUTTON0_PIN                         GPIO_PIN_0
#define USER_BUTTON0_GPIO_PORT                   GPIOH
#define USER_BUTTON0_EXTI_LINE                   GPIO_PIN_0
#define USER_BUTTON0_EXTI_IRQn                   EXTI0_IRQn
/* Button 1 - PH1*/
#define USER_BUTTON1_PIN                         GPIO_PIN_1
#define USER_BUTTON1_GPIO_PORT                   GPIOH
#define USER_BUTTON1_EXTI_LINE                   GPIO_PIN_1
#define USER_BUTTON1_EXTI_IRQn                   EXTI1_IRQn

typedef enum
{
  BUTTON_USER_0 = 0,
  BUTTON_USER_1 = 1
} Button_Type;

/*Functions*/
void Buttons_Init(ButtonMode_TypeDef ButtonMode);
uint32_t Button_GetState(Button_Type Button);

#endif /* BSP_BUTTONS_H_ */
