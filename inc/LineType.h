/*
 * LineType.h
 *
 *  Created on: 2016. dec. 7.
 *      Author: Muszka
 */

#ifndef LINETYPE_H_
#define LINETYPE_H_

#include "Nucleo.h"
#include "Leds.h"

/* V�ltoz�k */
//els� byte a vonalpoz�ci� (int8_t), a m�sodik a vonalak darabsz�ma(0,1,2,3)
extern uint8_t FrontSensor_Data[8];
//k�s�bb
extern uint8_t RearSensor_Data[8];
//vonalt�pus meghat�roz�s seg�dsz�ml�l�
extern uint16_t TimeLineType;
//sebess�g �llapot(StateQ1) ne v�ltozzon x m�sodpercig seg�dsz�ml�l�
extern uint16_t TimeSpeedState;
//adott pillanatban h�ny darab vonalat �ll�tunk
extern lineType LineNumber;
//�llapotv�ltoz�: kanyar vagy egyenes van �pp
extern speedState StateQ1;

/* F�ggv�nyek */
void GetLineType();
void SetSpeedState();
uint8_t IsElementsEqual(lineType* Array);

#endif /* LINETYPE_H_ */
