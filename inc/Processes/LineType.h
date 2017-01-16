/*
 * LineType.h
 *
 *  Created on: 2016. dec. 7.
 *      Author: Muszka
 */

#ifndef LINETYPE_H_
#define LINETYPE_H_

#include "Processes.h"

/* Külsõ változók */
//elsõ byte a vonalpozíció (int8_t), a második a vonalak darabszáma(0,1,2,3)
extern uint8_t FrontSensor_Data[8];	//CAN.c
//állapotvisszacsatolásos szabályzáshoz
extern uint8_t RearSensor_Data[8];	//CAN.c
//vonaltípus meghatározás segédszámláló
extern uint16_t TimeLineType;		//Timers.c

/* Publikus saját függvények */
void Do_LineType();
lineType Get_LineNumber();
State_LineType Get_StateLineType();


#endif /* LINETYPE_H_ */
