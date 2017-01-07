/*
 * Message.h
 *
 *  Created on: 2016. dec. 26.
 *      Author: Muszka
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "Processes.h"
//#include "UART.h"

//jelzi, hogy v�lozott-e a v�ltoz� �rt�ke a programban az el�z� kik�ld�se �ta
typedef enum{
	OldValue = 0,
	NewValue
}MessageStatus;

//�zenet strukt�ra a bluetooth-os debuggol�shoz
typedef struct{
	char* Name;
	float Data;
	MessageStatus Status;
}Messagetype;

//megfigyelt v�ltoz�k azonos�t�i az �zenetek t�mbj�ben, mindig a last_number legyen az utols�!
typedef enum{
	var_State = 0,
	var_P_Corner,
	var_P_Straight,
	var_D,
	var_LinePos,
	var_LineNumber,
	var_Speed_Corner,
	var_Speed_Straight,
	VariableName_LastElement
}VariableName;

/* V�ltoz�k */
extern Messagetype Value_MessageArray[];

/* F�ggv�yek */
void Do_Send_ValueMessageArray();
void SetValue_AtMessageArray(VariableName varname, const float data);
void Message_Init();
void SendDebugMessage_Raw(const char* string, const float data);
void SendDebugMessage_Text(const char* string);
void SendDebugMessage_Each(Messagetype* message);
void SendDebugMessage_Array(Messagetype* message, uint8_t arraysize);

#endif /* MESSAGE_H_ */