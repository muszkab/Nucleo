/*
 * LineType.h
 *
 *  Created on: 2016. dec. 7.
 *      Author: Muszka
 */

#ifndef LINETYPE_H_
#define LINETYPE_H_

#include "Processes.h"

/* K�ls� v�ltoz�k */
//els� byte a vonalpoz�ci� (int8_t), a m�sodik a vonalak darabsz�ma(0,1,2,3)
extern uint8_t FrontSensor_Data[8];	//CAN.c
//�llapotvisszacsatol�sos szab�lyz�shoz
extern uint8_t RearSensor_Data[8];	//CAN.c
//vonalt�pus meghat�roz�s seg�dsz�ml�l�
extern uint16_t TimeLineType;		//Timers.c

/* Publikus saj�t f�ggv�nyek */
void Do_LineType();
lineType Get_LineNumber();
State_LineType Get_StateLineType();


#endif /* LINETYPE_H_ */
