/*
 * Message.c
 *
 *  Created on: 2016. dec. 26.
 *      Author: Muszka
 */

#include "Message.h"

//�zenetk�ld�s ciklusideje
#define T_SENDMESSAGE	100	//T*1ms

//�zenett�mb m�rete, ennyi v�ltoz�t figyel�nk, enum darabsz�m�t�l f�gg a t�mb m�rete
#define  MESSAGEARRAY_SIZE	(VariableName_LastElement - 1)

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

		//amelyik v�ltoz� gyakran v�ltozik(pl. LinePos), azt itt �ll�tjuk, amelyik ritk�n, azt helyben, hogyha v�ltozott
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

void Message_Init()
{
	//t�mb kit�lt�se a nevekkel, amik alapj�n a qt szoftver azonos�tja az egyes �zeneteket
	Value_MessageArray[var_State].Name			= "Value:Status";
	Value_MessageArray[var_P_Corner].Name		= "Value:P_Corner";
	Value_MessageArray[var_P_Straight].Name 	= "Value:P_Straight";
	Value_MessageArray[var_D].Name				= "Value:D";
	Value_MessageArray[var_LinePos].Name		= "Value:LinePos";
	Value_MessageArray[var_LineNumber].Name		= "Value:LineNumber";
	Value_MessageArray[var_LinePos].Name		= "Value:LinePos";
	Value_MessageArray[var_Speed_Corner].Name	= "Value:Speed_Corner";
	Value_MessageArray[var_Speed_Straight].Name	= "Value:Speed_Straight";
	//status �s adat null�z�sa
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
