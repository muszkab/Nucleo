/*
 * MotorControl.c
 *
 *  Created on: 2017. jan. 5.
 *      Author: szabgyul92
 */

/* Includes ------------------------------------------------------------------*/
#include "MotorControl.h"

#define MAX_OUTPUT	100.0f //Az inverz függvényben a maximális megengedhetõ kimenet FOXBORO-hoz
#define Zd			0.98f
#define Kc			3.0f


#define LOOKUP_MAX	10   //Identifikációs mérések száma
//Inverz karakterisztika y tengelye
static const float LookUpU[LOOKUP_MAX] = {
		0,	0,	0,	0,	0,	0,	0,	0,	0,	0};
//Inverz karakterisztika x tengelye
static const float LookUpY[LOOKUP_MAX] = {
		0,	0,	0,	0,	0,	0,	0,	0,	0,	0};
static const float Offset = 5;
static const float m      = 1;
/* Szabályzó változók FOXBORO PI */
float u1;
float u2;
float u;
float Velocity;

void MotorControl(float VelocityRef){
	if(TimeMotorControl > 100){
		TimeMotorControl = 0;
		/* Algoritmus */
		u2 = Zd * u2 + (1- Zd)*u;
		Velocity = Encoder_GetVelocity();
		u1 = Kc * (VelocityRef - Velocity);
		u = MotorControlSaturate(u1+u2);
		SetSpeed((int8_t)LookUpTable(u));
	}
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
