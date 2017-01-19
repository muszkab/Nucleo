/*
 * Message.h
 *
 *  Created on: 2016. dec. 26.
 *      Author: Muszka
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "Processes.h"

//jelzi, hogy változott-e a változó értéke a programban az elõzõ kiküldése óta
typedef enum{
	OldValue = 0,
	NewValue
}MessageStatus;

//üzenet struktúra a bluetooth-os debuggoláshoz
typedef struct{
	char* Name;
	float Data;
	MessageStatus Status;
}Messagetype;

//megfigyelt változók azonosítói az üzenetek tömbjében, mindig a last_number legyen az utolsó!
typedef enum{
	var_State = 0,
	var_P_Corner,
	var_P_Straight,
	var_D,
	var_LinePos,
	var_LineNumber,
	var_SpeedNow,
	var_Speed_Corner,
	var_Speed_Straight,
	var_BatteryVoltage,
	var_FrontSharp,
	var_RightSharp,
	var_LeftSharp,
	var_Distance,
	var_OneLineDistance,
	var_TwoLineDistance,
	var_ThreeLineDistance,
	var_StateLineType,
	var_StateWall,
	var_TrackElement,
	var_DegreeX,
	var_DegreeY,
	var_DegreeZ,
	var_VelocityRef,
	VariableName_LastElement
}VariableName;

/* Változók */
extern Messagetype Value_MessageArray[];

/* Függvéyek */
void Do_Send_ValueMessageArray();
void SetValue_AtMessageArray(VariableName varname, const float data);
void Message_Init();
void SendDebugMessage_Raw(const char* string, const float data);
void SendDebugMessage_Text(const char* string);
void SendDebugMessage_Each(Messagetype* message);
void SendDebugMessage_Array(Messagetype* message, uint8_t arraysize);

#endif /* MESSAGE_H_ */
