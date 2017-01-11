/*
 * Message.c
 *
 *  Created on: 2016. dec. 26.
 *      Author: Muszka
 */

#include "Message.h"

//�zenetk�ld�s ciklusideje
#define T_SENDMESSAGE	500	//T*1ms

//�zenett�mb m�rete, ennyi v�ltoz�t figyel�nk, enum darabsz�m�t�l f�gg a t�mb m�rete
#define  MESSAGEARRAY_SIZE	VariableName_LastElement

//pillanatnyi �rt�k fel�letre("#Value") kik�ld�tt v�ltoz�k �zenett�mbje
Messagetype Value_MessageArray[MESSAGEARRAY_SIZE];
//seg�dsz�ml�l� (Timers.c)
extern uint16_t TimeSendMessage;

//�zenetk�ld�s ciklikusan, csak a megv�ltozott �rt�keket k�ldj�k el
void Do_Send_ValueMessageArray()
{
	if(TimeSendMessage > T_SENDMESSAGE)
	{
		TimeSendMessage = 0;

		//amelyik v�ltoz� gyakran v�ltozik(pl. LinePosition), azt itt �ll�tjuk, amelyik ritk�n, azt helyben, hogyha v�ltozott
		SetValue_AtMessageArray(var_LineNumber,	(float) Get_LineNumber());
		SetValue_AtMessageArray(var_LinePos,	(float) Get_LinePosition());

		SetValue_AtMessageArray(var_Distance,	(float) Encoder_GetDistance_cm());
		SetValue_AtMessageArray(var_SpeedNow,	(float) Encoder_GetVelocity());

		SetValue_AtMessageArray(var_P_Corner,	(float) Get_P_Corner());
		SetValue_AtMessageArray(var_P_Straight,	(float) Get_P_Straight());
		SetValue_AtMessageArray(var_D,			(float) Get_D());

		SetValue_AtMessageArray(var_FrontSharp,	(float) GetFrontSharpDistance());
		SetValue_AtMessageArray(var_RightSharp,	(float) GetRightSharpDistance());
		SetValue_AtMessageArray(var_LeftSharp,	(float) GetLeftSharpDistance());

		SetValue_AtMessageArray(var_BatteryVoltage, (float) GetBatteryVoltage());

		SetValue_AtMessageArray(var_ThreeLineDistance, (float) Get_ThreeLineDistance());

		//SetValue_AtMessageArray(var_State, (float) StateQ1);

		for(int i=0;i<MESSAGEARRAY_SIZE;i++)
		{
			if(Value_MessageArray[i].Status == NewValue)
			{
				Value_MessageArray[i].Status = OldValue;
				SendDebugMessage_Each(Value_MessageArray+i);
			}
		}
	}
}

void Message_Init()
{
	//t�mb kit�lt�se a nevekkel, amik alapj�n a qt szoftver azonos�tja az egyes �zeneteket
	Value_MessageArray[var_State].Name			= "Value:State";

	Value_MessageArray[var_LinePos].Name		= "Value:LinePos";
	Value_MessageArray[var_LineNumber].Name		= "Value:LineNumber";

	Value_MessageArray[var_SpeedNow].Name		= "Value:SpeedNow";
	Value_MessageArray[var_Distance].Name		= "Value:Distance";

	Value_MessageArray[var_P_Corner].Name		= "Value:P_Corner";
	Value_MessageArray[var_P_Straight].Name 	= "Value:P_Straight";
	Value_MessageArray[var_D].Name				= "Value:D";

	Value_MessageArray[var_BatteryVoltage].Name	= "Value:BatteryVoltage";

	Value_MessageArray[var_FrontSharp].Name		= "Value:FrontSharp";
	Value_MessageArray[var_RightSharp].Name		= "Value:RightSharp";
	Value_MessageArray[var_LeftSharp].Name		= "Value:LeftSharp";

	Value_MessageArray[var_ThreeLineDistance].Name = "Value:ThreeLineD";

	//status �s adat null�z�sa
	for(int i =0;i<MESSAGEARRAY_SIZE;i++)
	{
		Value_MessageArray[i].Data = 0;
		Value_MessageArray[i].Status = NewValue;
	}
}

//v�ltoz��rt�k be�ll�t�s a t�mbben, abban a f�jlban kell h�vni, ahol az eredeti v�ltoz� van. Csak akkor fut le, ha �j az �rt�k.
void SetValue_AtMessageArray(VariableName varname, const float data)
{
	//megv�ltozott a figyelt v�ltoz� �rt�ke
	if(data != Value_MessageArray[varname].Data)
	{
		Value_MessageArray[varname].Data = data;
		Value_MessageArray[varname].Status = NewValue;
	}
}

void SendDebugMessage_Raw(const char* string, const float data)
{
	printf("#%s:%3.3f;\n", string, data);
	printf("\r");
}

void SendDebugMessage_Text(const char* string)
{
	printf("#Text:%s;\n", string);
	printf("\r");
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
