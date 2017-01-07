/*
 * LineType.h
 *
 *  Created on: 2016. dec. 7.
 *      Author: Muszka
 */

#ifndef LINETYPE_H_
#define LINETYPE_H_

#include "Processes.h"

/* Változók */
//elsõ byte a vonalpozíció (int8_t), a második a vonalak darabszáma(0,1,2,3)
extern uint8_t FrontSensor_Data[8];
//késõbb
extern uint8_t RearSensor_Data[8];
//vonaltípus meghatározás segédszámláló
extern uint16_t TimeLineType;
//sebesség állapot(StateQ1) ne változzon x másodpercig segédszámláló
extern uint16_t TimeSpeedState;
//adott pillanatban hány darab vonalat állítunk
//extern lineType LineNumber;

//TODO: törlés!
//állapotváltozó: kanyar vagy egyenes van épp
extern speedState StateQ1;

/* Függvények */
void Do_GetLineType();
void Is_EgyVonal();
void Is_HaromVonal();
void SetSpeedState();
uint8_t IsElementsEqual(lineType* Array);
/* Külsõ függvények */
float Encoder_GetDistance();     // m ; Encoder.c
void SetState();

#endif /* LINETYPE_H_ */
