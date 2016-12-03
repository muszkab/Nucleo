/*
 * PositionControl.h
 *
 *  Created on: 2016. dec. 1.
 *      Author: Muszka
 */

#ifndef POSITIONCONTROL_H_
#define POSITIONCONTROL_H_

#include "Nucleo.h"

/* V�ltoz�k */
//els� byte a vonalpoz�ci� (int8_t), a m�sodik a vonalak darabsz�ma(0,1,2,3)
extern uint8_t FrontSensor_Data[8];
//k�s�bb
extern uint8_t RearSensor_Data[8];
//beavatkoz� jel
extern uint8_t ServoPos;
//hibajel, mostani[0] �s el�z�[1] �rt�k
int8_t LinePosition[2];
//szab�lyz� seg�dsz�ml�l�
extern uint32_t Szabcount;

/*F�ggv�nyek  */
void SetServoPWMPulse(const int8_t ServoPos);
void Do_PositionControl();

#endif /* POSITIONCONTROL_H_ */
