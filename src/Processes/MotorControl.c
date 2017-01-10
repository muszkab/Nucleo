/*
 * MotorControl.c
 *
 *  Created on: 2017. jan. 5.
 *      Author: szabgyul92
 */

/* Includes ------------------------------------------------------------------*/
#include "MotorControl.h"

#define MAX_OUTPUT	80.0f //Az inverz függvényben a maximális megengedhetõ kimenet FOXBORO-hoz
#define Zd			0.9f
#define Kc			8.78f


#define LOOKUP_MAX	10   //Identifikációs mérések száma
//Inverz karakterisztika y tengelye
/* static const float LookUpU[LOOKUP_MAX] = {
		0,	0,	0,	0,	0,	0,	0,	0,	0,	0};
//Inverz karakterisztika x tengelye
static const float LookUpY[LOOKUP_MAX] = {
		0,	0,	0,	0,	0,	0,	0,	0,	0,	0}; */
static const float Offset = 16;
static const float m      = 1;
/* Szabályzó változók FOXBORO PI */
static float u1;
static float u2;
static float u;
static float Velocity;
static float VelocityRef;
/* Identifikációhoz */
uint8_t IdentificationEnable = 1;

void MotorControl(){
	if(TimeMotorControl > 20){
		TimeMotorControl = 0;
		/* Algoritmus */
		u2 = Zd * u2 + (1- Zd)*u;
		Velocity = Encoder_GetVelocity();
		u1 = Kc * (VelocityRef - Velocity);
		u = MotorControlSaturate(u1+u2);
		SetSpeedFactory((int8_t)LookUpTable(u));
	}
}

void MotorControlSetVelocityRef(float Vref){
	VelocityRef = Vref;
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
	if(u < 0){
		sign = -1;
		u *= -1;
	}
	return sign*(m*u+Offset);
}

/**
  * @brief  Motor identifikáció
  * A megadott tartományon a megadott idõközökkel gerjeszti a motort és elküldi
  * bluetoothon a sebességértéket.
  * Identifikálásnál csak ez a függvény fusson
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
