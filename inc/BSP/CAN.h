/*
 * CAN.h
 *
 *  Created on: 2016. okt. 20.
 *      Author: Muszka
 */

#ifndef CAN_H_
#define CAN_H_

#include "Nucleo.h"

/* Definition for CANx clock resources */
#define CANx                            CAN1
#define CANx_CLK_ENABLE()               __HAL_RCC_CAN1_CLK_ENABLE()
#define CANx_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOA_CLK_ENABLE()

#define CANx_FORCE_RESET()              __HAL_RCC_CAN1_FORCE_RESET()
#define CANx_RELEASE_RESET()            __HAL_RCC_CAN1_RELEASE_RESET()

/* Definition for CANx Pins */
#define CANx_TX_PIN                    GPIO_PIN_12
#define CANx_TX_GPIO_PORT              GPIOA
#define CANx_TX_AF                     GPIO_AF9_CAN1
#define CANx_RX_PIN                    GPIO_PIN_11
#define CANx_RX_GPIO_PORT              GPIOA
#define CANx_RX_AF                     GPIO_AF9_CAN1

/*Global variables*/
extern CAN_HandleTypeDef CanHandle;
extern uint8_t FrontSensor_Data[8];
extern uint8_t RearSensor_Data[8];
/*Functions*/
void CAN_Init();
HAL_StatusTypeDef CAN_TransmitIT(uint8_t *data, uint8_t length);
void CAN_ReceiveIT();

#endif /* CAN_H_ */
