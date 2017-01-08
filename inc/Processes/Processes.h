/*
 * Processes.h
 *
 *  Created on: 2017. jan. 2.
 *      Author: Muszka
 */

#ifndef PROCESSES_PROCESSES_H_
#define PROCESSES_PROCESSES_H_

#include "Nucleo.h"

/*Global types*/
typedef enum{
	NoLine = 0,
	OneLine = 1,
	TwoLine = 2,
	ThreeLine =3
}lineType;

typedef enum{
	Stop,
	CornerIn,
	CornerOut,
	Straight
}speedState;

typedef enum{
	Egyvonal_folyt = 0,
	Egyvonal_szagg,
	Ketvonal_x,
	Ketvonal_folyt,
	Ketvonal_szagg,
	Haromvonal_x,
	Haromvonal_folyt,
	Haromvonal_szagg
}State_LineType;

/*Get függvények*/
/* LineType.h */
extern lineType Get_LineNumber();
extern State_LineType Get_StateLineType();
/* PositionControl.h */
extern int8_t Get_ServoPosition();
extern int8_t Get_LinePosition();
extern float Get_P_Corner();
extern float Get_P_Straight();
extern float Get_D();

//állapotváltozó: kanyar vagy egyenes van épp
extern speedState StateQ1;


#endif /* PROCESSES_PROCESSES_H_ */
