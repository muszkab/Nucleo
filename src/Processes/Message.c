/*
 * Message.c
 *
 *  Created on: 2016. dec. 26.
 *      Author: Muszka
 */

#include "Message.h"

void SendDebugMessage_Raw(const char* string, const float data)
{
	printf("#%s:%3.3f;\n\r", string, data);
}

void SendDebugMessage_Each(Messagetype* message)
{
	SendDebugMessage_Raw(message->Name, message->Data);
}

void SendDebugMessage_Array(Messagetype* message, uint8_t arraysize)
{
	for(int i=0;i<arraysize;i++)
		SendDebugMessage_Each(message+i);
}
