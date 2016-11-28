/*
 * CAN.c
 *
 *  Created on: 2016. okt. 20.
 *      Author: Muszka
 */

#include "CAN.h"

/*CAN handler*/
CAN_HandleTypeDef CanHandle;
/* Vonalszenzortól érkezõ adatok, max 8 byte a CAN miatt */
uint8_t FrontSensor_Data[8];
uint8_t RearSensor_Data[8];

/*Headerben kell meghatározni melyik CAN, Port és PIN */
void CAN_Init()
{
	GPIO_InitTypeDef   GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* CAN1 Periph clock enable */
	CANx_CLK_ENABLE();
	/* Enable GPIO clock ****************************************/
	CANx_GPIO_CLK_ENABLE();

	/*##-2- Configure peripheral GPIO ##########################################*/
	/* CAN1 TX GPIO pin configuration */
	GPIO_InitStruct.Pin = CANx_TX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Alternate =  CANx_TX_AF;

	HAL_GPIO_Init(CANx_TX_GPIO_PORT, &GPIO_InitStruct);

	/* CAN1 RX GPIO pin configuration */
	GPIO_InitStruct.Pin = CANx_RX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Alternate =  CANx_RX_AF;

	HAL_GPIO_Init(CANx_TX_GPIO_PORT, &GPIO_InitStruct);

	CAN_FilterConfTypeDef  		sFilterConfig;
	static CanTxMsgTypeDef      TxMessage;
	static CanRxMsgTypeDef      RxMessage;

	/*##-3- Configure the CAN peripheral #######################################*/
	CanHandle.Instance = CANx;
	CanHandle.pTxMsg = &TxMessage;
	CanHandle.pRxMsg = &RxMessage;

	CanHandle.Init.TTCM = DISABLE;
	CanHandle.Init.ABOM = DISABLE;
	CanHandle.Init.AWUM = DISABLE;
	CanHandle.Init.NART = DISABLE;
	CanHandle.Init.RFLM = DISABLE;
	CanHandle.Init.TXFP = DISABLE;
	/* Átállítani normálba! */
	CanHandle.Init.Mode = CAN_MODE_NORMAL;
	CanHandle.Init.SJW = CAN_SJW_2TQ;
	CanHandle.Init.BS1 = CAN_BS1_6TQ;
	CanHandle.Init.BS2 = CAN_BS2_8TQ;
	CanHandle.Init.Prescaler = 5;

	if(HAL_CAN_Init(&CanHandle) != HAL_OK)
	{
	/* Initialization Error */
		Error_SendUart("CAN_Init hiba, nincs tap alatt a CAN illeszto!");
	}

	/*##-4- Configure the CAN Filter ###########################################*/
	sFilterConfig.FilterNumber = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = 0x0000;
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = 0;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.BankNumber = 14;

	if(HAL_CAN_ConfigFilter(&CanHandle, &sFilterConfig) != HAL_OK)
	{
	/* Filter configuration Error */
	Error_Handler();
	}

	/* Interrupt conf */
	HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0x3, 0x0);
	HAL_NVIC_SetPriority(CAN1_TX_IRQn, 0x3, 0x0);
	HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
}

HAL_StatusTypeDef CAN_TransmitIT(uint8_t *data, uint8_t length)
{
	  if(length > 8 || length < 1)
		  return HAL_ERROR;

	  CanHandle.pTxMsg->StdId = 0x01;
	  CanHandle.pTxMsg->RTR = CAN_RTR_DATA;
	  CanHandle.pTxMsg->IDE = CAN_ID_STD;
	  CanHandle.pTxMsg->DLC = length;
	  for(int i=0;i<length;i++)
		  CanHandle.pTxMsg->Data[i] = data[i];

	  if(HAL_CAN_Transmit(&CanHandle,10) != HAL_OK)
	  {
	    /* Transmition Error */
	    Error_Handler();
	  }
	  return HAL_OK; /* Test Passed */
}

/* Üzenetkérés a vonalszenzoroktól */
HAL_StatusTypeDef CAN_ReceiveIT()
{
	  if(HAL_CAN_Receive_IT(&CanHandle, CAN_FIFO0) != HAL_OK)
	  {
	    /* Reception Error */
	    Error_SendUart("CAN illeszto nem jo!");
	  }
	  return HAL_OK;
}

/* HAL_CAN_Receive_IT hívja meg, ha kész a CAN üzenet fogadás */
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{
	if(hcan->ErrorCode == HAL_CAN_ERROR_NONE)
	{
		uint8_t length=hcan->pRxMsg->DLC;
		if(hcan->pRxMsg->StdId == 1)
		{
			for(int i=0;i<length;i++)
				FrontSensor_Data[i]=hcan->pRxMsg->Data[i];
			for(int i=length;i<8;i++)
				FrontSensor_Data[i]=0;
		}
		if(hcan->pRxMsg->StdId == 2)
		{
			for(int i=0;i<length;i++)
				RearSensor_Data[i]=hcan->pRxMsg->Data[i];
			for(int i=length;i<8;i++)
				RearSensor_Data[i]=0;
		}
	}
	CAN_ReceiveIT();
}

