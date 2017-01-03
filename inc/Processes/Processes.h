/*
 * Processes.h
 *
 *  Created on: 2017. jan. 2.
 *      Author: Muszka
 */

#ifndef PROCESSES_PROCESSES_H_
#define PROCESSES_PROCESSES_H_

#include "Message.h"

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
//állapotváltozó: kanyar vagy egyenes van épp
extern speedState StateQ1;

#endif /* PROCESSES_PROCESSES_H_ */
