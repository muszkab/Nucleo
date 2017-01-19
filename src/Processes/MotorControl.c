/*
 * MotorControl.c
 *
 *  Created on: 2017. jan. 5.
 *      Author: szabgyul92
 */

/* Includes ------------------------------------------------------------------*/
#include "MotorControl.h"

#define T_MOTORCONTROL	20
#define MAX_OUTPUT	47000//61200.0f //Az inverz f�ggv�nyben a maxim�lis megengedhet� kimenet FOXBORO-hoz
static float Zd = 0.96f;
static float Kc = 1.5f;

#define VELOCITY_LIMIT			200
#define ACCELERATION_LIMIT		10

#define LOOKUP_MAX	10   //Identifik�ci�s m�r�sek sz�ma
//Inverz karakterisztika y tengelye
/* static const float LookUpU[LOOKUP_MAX] = {
		0,	0,	0,	0,	0,	0,	0,	0,	0,	0};
//Inverz karakterisztika x tengelye
static const float LookUpY[LOOKUP_MAX] = {
		0,	0,	0,	0,	0,	0,	0,	0,	0,	0}; */
static float Offset = 30;
static const float m      = 0.0067;
/* Szab�lyz� v�ltoz�k FOXBORO PI */
static float u1;
static float u2;
static float u;
static float Velocity;
static float VelocityRef;
static int8_t VelocityPolarity = 1;

/* Identifik�ci�hoz */
uint8_t IdentificationEnable = 1;

void Do_MotorControl(){
	if(TimeMotorControl > T_MOTORCONTROL){
		TimeMotorControl = 0;

		//enk�der �ltal m�rt sebess�g
		Velocity = Encoder_GetVelocityRaw();

		if(VelocityRef == 0){
			if(Velocity < 1700 && Velocity > -1700){
				Offset = 0;
				if(Velocity < - 1100){
					VelocityPolarity = -1;
				}
				if(Velocity > 1100){
					VelocityPolarity = 1;
				}
				Zd = 0.6;
				Kc = 3;
			}

		}
		else{
			Offset = 30.0;
			Zd = 0.96;
			Kc = 1.5;
		}

		/* Algoritmus */
		u2 = Zd * u2 + (1- Zd)*u;
		u1 = Kc * (VelocityRef - Velocity);
		if(VelocityRef == 0){
			if(Velocity < 1700 && Velocity > -1700){
				if(VelocityPolarity == 1){
					if(u2+u1 > 0){
						u2=0;
						u1=0;
					}
				}
				if(VelocityPolarity == -1){
					if(u2+u1 < 0){
						u2=0;
						u1=0;
					}
				}
			}
		}

		u = MotorControlSaturate(u1+u2);
		SetSpeed((int16_t)LookUpTable(u));
	}
}

void MotorControlSetVelocityRef(float Vref){
	VelocityRef = Vref/METER_PER_INCR;
}

float MotorControlGetVelocityRef(){
	return VelocityRef*METER_PER_INCR;
}

float MotorControlSaturate(float u){
	if(u > MAX_OUTPUT)
		return MAX_OUTPUT;
	if(u < -MAX_OUTPUT)
		return -MAX_OUTPUT;
	return u;
}
float LookUpTable(float u){
	int8_t sign = 1;
	float y;
	if(u < 0){
		sign = -1;
		u *= -1;
	}
	y = m*u+Offset;
	if(y <= Offset)
		y = 0;
	return sign*y;
}

/**
  * @brief  Motor identifik�ci�
  * A megadott tartom�nyon a megadott id�k�z�kkel gerjeszti a motort �s elk�ldi
  * bluetoothon a sebess�g�rt�ket.
  * Identifik�l�sn�l csak ez a f�ggv�ny fusson
  */
void MotorIdentification(){
	int16_t output = 0;
	uint16_t time_run = 0;
	uint16_t time_wait = 0;
	float    velocity;
	for(output = START_VALUE; output <= END_VALUE; output += STEP ){
		while(IdentificationEnable){}
		SetSpeed(output);
		for(time_run = 0; time_run < TIME_RUN; time_run+=TIME_STEP_RUN){
			HAL_Delay(TIME_STEP_RUN);
			velocity = Encoder_GetVelocityRaw();
			printf("%3.3f; %d\n\r", velocity, output);
			if(Is_StopCommand())
				SetSpeed(0);
		}
		SetSpeed(0);
		for(time_wait = 0; time_wait < TIME_WAIT; time_wait+=TIME_STEP_WAIT){
			HAL_Delay(TIME_STEP_WAIT);
			velocity = Encoder_GetVelocityRaw();
			printf("%3.3f; 0\n\r", velocity);
		}
		IdentificationEnable = 1;
	}

}

