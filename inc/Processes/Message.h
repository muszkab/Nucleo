/*
 * Message.h
 *
 *  Created on: 2016. dec. 26.
 *      Author: Muszka
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "UART.h"

typedef struct{
	char* Name;
	float Data;
}Messagetype;

void SendDebugMessage_Raw(const char* string, const float data);
void SendDebugMessage_Each(Messagetype* message);
void SendDebugMessage_Array(Messagetype* message, uint8_t arraysize);

#endif /* MESSAGE_H_ */
