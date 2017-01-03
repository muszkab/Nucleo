/*
 * Processes.h
 *
 *  Created on: 2017. jan. 2.
 *      Author: Muszka
 */

#ifndef PROCESSES_PROCESSES_H_
#define PROCESSES_PROCESSES_H_

#include "Nucleo.h"
//#include "Message.h"

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

/*Global variables*/
//�llapotv�ltoz�: kanyar vagy egyenes van �pp
extern speedState StateQ1;
//adott pillanatban h�ny darab vonalat �ll�tunk
extern lineType LineNumber;
//vonalpoz�ci� hibajel, mostani[0] �s el�z�[1] �rt�k
extern int8_t LinePosition[];

#endif /* PROCESSES_PROCESSES_H_ */
