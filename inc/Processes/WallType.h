/*
 * WallType.h
 *
 *  Created on: 2017. jan. 16.
 *      Author: Muszka
 */

#ifndef PROCESSES_WALLTYPE_H_
#define PROCESSES_WALLTYPE_H_

#include "Processes.h"

/* Külsõ változók */
//faltípus felismerés segédszámláló
extern uint16_t TimeWallType;		//Timers.c

/* Publikus saját függvények */
void Do_WallType();
State_Wall Get_StateWall();
void Set_StateWall(State_Wall newStateWall);


#endif /* PROCESSES_WALLTYPE_H_ */
