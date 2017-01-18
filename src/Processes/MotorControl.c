/*
 * MotorControl.c
 *
 *  Created on: 2017. jan. 5.
 *      Author: szabgyul92
 */

/* Includes ------------------------------------------------------------------*/
#include "MotorControl.h"

#define MAX_OUTPUT	61200.0f //Az inverz függvényben a maximális megengedhetõ kimenet FOXBORO-hoz
static float Zd = 0.96f;
static float Kc = 1.0f;

#define VELOCITYFILTER_LIMIT	4000 //nemtom mekkora szám kell
#define ACCELERATION_LIMIT		10

#define LOOKUP_MAX	10   //Identifikációs mérések száma
//Inverz karakterisztika y tengelye
/* static const float LookUpU[LOOKUP_MAX] = {
		0,	0,	0,	0,	0,	0,	0,	0,	0,	0};
//Inverz karakterisztika x tengelye
static const float LookUpY[LOOKUP_MAX] = {
		0,	0,	0,	0,	0,	0,	0,	0,	0,	0}; */
static const float Offset = 30;
static const float m      = 0.0067;
/* Szabályzó változók FOXBORO PI */
static float u1;
static float u2;
static float u;
static float Velocity;
static float VelocityRef;

/* Identifikációhoz */
uint8_t IdentificationEnable = 1;

void Do_MotorControl(){
	if(TimeMotorControl > 20){
		TimeMotorControl = 0;

		Velocity = Encoder_GetVelocityRaw();
		/* Algoritmus */
		u2 = Zd * u2 + (1- Zd)*u;
		u1 = Kc * (VelocityRef - Velocity);
		u = MotorControlSaturate(u1+u2);
		SetSpeed((int16_t)LookUpTable(u));
	}
}

//bele kell rakni a MotorControl()-ba
void ZeroSpeedFilter_Vel(float* EncoderVelocity, const float* VelocityRef)
{
	if((*VelocityRef == 0) && (*EncoderVelocity > VELOCITYFILTER_LIMIT))
		*EncoderVelocity = VELOCITYFILTER_LIMIT;
}

void ZeroSpeedFilter_Acc(float* EncoderVelocity, const float* VelocityRef, const float* EncoderAcceleration)
{
	if((*VelocityRef == 0) && (*EncoderVelocity > VELOCITYFILTER_LIMIT) && (*EncoderAcceleration > ACCELERATION_LIMIT))
		*EncoderVelocity = VELOCITYFILTER_LIMIT;
}

void MotorControlSetVelocityRef(float Vref){
	VelocityRef = Vref/METER_PER_INCR;
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

