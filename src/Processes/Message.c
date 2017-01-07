/*
 * Message.c
 *
 *  Created on: 2016. dec. 26.
 *      Author: Muszka
 */

#include "Message.h"

//üzenetküldés ciklusideje
#define T_SENDMESSAGE	100	//T*1ms

//üzenettömb mérete, ennyi változót figyelünk, enum darabszámától függ a tömb mérete
#define  MESSAGEARRAY_SIZE	(VariableName_LastElement - 1)

//pillanatnyi érték felületre("#Value") kiküldött változók üzenettömbje
Messagetype Value_MessageArray[MESSAGEARRAY_SIZE];
//segédszámláló (Timers.c)
extern uint16_t TimeSendMessage;

//üzenetküldés ciklikusan, csak a megváltozott értékeket küldjük el
void Do_Send_ValueMessageArray()
{
	if(TimeSendMessage > T_SENDMESSAGE)
	{
		TimeSendMessage = 0;

		//amelyik változó gyakran változik(pl. LinePos), azt itt állítjuk, amelyik ritkán, azt helyben, hogyha változott
		SetValue_AtMessageArray(var_LineNumber, (float)LineNumber);
		SetValue_AtMessageArray(var_LinePos, (float)LinePosition[0]);
		//SetValue_AtMessageArray(var_State, (float) StateQ1);
		//SetValue_AtMessageArray(var_Speed_Corner, (float) StateQ1);
		//SetValue_AtMessageArray(var_Speed_Straight, (float) StateQ1);
		//SetValue_AtMessageArray(var_P_Corner, (float) StateQ1);
		//SetValue_AtMessageArray(var_P_Straight, (float) StateQ1);

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

//változóérték beállítás a tömbben, abban a fájlban kell hívni, ahol az eredeti változó van. Csak akkor fut le, ha új az érték.
void SetValue_AtMessageArray(VariableName varname, const float data)
{
	//megváltozott a figyelt változó értéke
	if(data != Value_MessageArray[varname].Data)
	{
		Value_MessageArray[varname].Data = data;
		Value_MessageArray[varname].Status = NewValue;
	}
}

void Message_Init()
{
	//tömb kitöltése a nevekkel, amik alapján a qt szoftver azonosítja az egyes üzeneteket
	Value_MessageArray[var_State].Name			= "Value:Status";
	Value_MessageArray[var_P_Corner].Name		= "Value:P_Corner";
	Value_MessageArray[var_P_Straight].Name 	= "Value:P_Straight";
	Value_MessageArray[var_D].Name				= "Value:D";
	Value_MessageArray[var_LinePos].Name		= "Value:LinePos";
	Value_MessageArray[var_LineNumber].Name		= "Value:LineNumber";
	Value_MessageArray[var_LinePos].Name		= "Value:LinePos";
	Value_MessageArray[var_Speed_Corner].Name	= "Value:Speed_Corner";
	Value_MessageArray[var_Speed_Straight].Name	= "Value:Speed_Straight";
	//status és adat nullázása
	for(int i =0;i<MESSAGEARRAY_SIZE;i++)
	{
		Value_MessageArray[i].Data = 0;
		Value_MessageArray[i].Status = OldValue;
	}
}

void SendDebugMessage_Raw(const char* string, const float data)
{
	printf("#%s:%3.3f;\n\r", string, data);
}

void SendDebugMessage_Text(const char* string)
{
	printf("#Text:%s;\n\r", string);
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
