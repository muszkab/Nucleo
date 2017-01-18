/*
 * Processes.h
 *
 *  Created on: 2017. jan. 2.
 *      Author: Muszka
 */

#ifndef PROCESSES_PROCESSES_H_
#define PROCESSES_PROCESSES_H_

#include <BSP.h>

/*Global types*/
typedef enum{
	NoLine,
	OneLine,
	TwoLine,
	ThreeLine,
	Full
}lineType;

typedef enum{
	Stop,
	CornerIn,
	CornerOut,
	Straight
}speedState;

typedef enum{
	Egyvonal_folyt,
	Egyvonal_szagg,
	Ketvonal_x,
	Ketvonal_folyt,
	Ketvonal_szagg,
	Haromvonal_x,
	Haromvonal_folyt,
	Haromvonal_szagg,
	Keresztvonal
}State_LineType;

typedef enum{
	Nincsfal,
	Kapu,
	Fal,
	Simafal,
	Simafal_jobb,
	Simafal_bal,
	Bordasfal,
	Bordasfal_jobb,
	Bordasfal_bal,
	Korforgalom_jobb,
	Korforgalom_bal,
	Hiba
}State_Wall;

typedef enum{
	Normal_E,
	Start_E,
	Finish_E,
	Dron,
	TrailerOn,
	TrailerOff,
	Zebra,
	Teeter,
	Barrel,
	Roundabout
}State_TrackElement;

/* Message függvények */
extern void SendDebugMessage_Text(const char* string);
/* RemoteControl Stop */
extern int8_t Is_StopCommand();

/*Get függvények*/
/* LineType.h */
extern lineType Get_LineNumber();
extern State_LineType Get_StateLineType();
extern float Get_ThreeLineDistance();
/* PositionControl.h */
extern int8_t Get_ServoPosition();
extern int8_t Get_LinePosition();
extern float Get_P_Corner();
extern float Get_P_Straight();
extern float Get_D();
/* Sharp.h */
extern float GetBatteryVoltage();
extern float GetFrontSharpDistance();
extern float GetRightSharpDistance();
extern float GetLeftSharpDistance();
/* Encoder.h */
extern float Encoder_GetDistance_cm();	 	// cm
extern float Encoder_GetVelocity(void); 	// m/s
extern float Encoder_GetVelocityRaw(void);	// inkrement/s
extern float Encoder_GetAcceleration(void);
/* WallType.h */
extern State_Wall Get_StateWall();
/* TrackElement.h */
extern State_TrackElement Get_TrackElement();

//állapotváltozó: kanyar vagy egyenes van épp
extern speedState StateQ1;


#endif /* PROCESSES_PROCESSES_H_ */
