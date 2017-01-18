/*
 * PositionControl.c
 *
 *  Created on: 2016. dec. 1.
 *      Author: Muszka
 */

#include "PositionControl.h"
#include "math.h"

//szab�lyz� param�terek
#define	P_CORNER	1.5f
#define	P_STRAIGHT	1.0f
#define D			20.0f
#define T			10		//T*1ms

/* static v�ltoz�k */
//beavatkoz� jel
static float ServoPosition=0;
//P �rt�ke att�l f�gg�en milyen �llapotban vagyunk: kanyarban magassabb, egyenesben kisebb P
static float P			= P_CORNER;
static float P_Corner	= P_CORNER;
static float P_Straight	= P_STRAIGHT;
//D tag, jelenleg ugyanaz az �rt�k mindenhol
static float D_All		= D;
//hibajel, mostani[0] �s el�z�[1] �rt�k
static int8_t LinePosition[2];

/*************************************************/
/************ �LLAPOTTERES SZAB�LYZ�**************/
/*************************************************/
//TODO Poz�ci� -128..127, sz�g -60..60, szervo -128..127. Sz�g nincs azonos tartom�nyban, meg kell oldani
#define 	 Ksi		0.9		// Csillap�t�si t�nyez�, aj�nlott �rt�ke : 0,7 0,9
#define 	 T_AT 		10 		// Ciklusid� [ms]
#define		 L			0.385f	// H�ts� ker�k - szenzorsor t�vols�g [m]
static float  D5Percent;	// Be�ll�si �thossz [m]
static float  AT_T5Percent;			// Be�ll�si �thosszhoz tartoz� id�							 [m/s]
static float  AT_Orient;			// orient�ci�
static float  AT_Pos;				// poz�ci� a Front vonalszenzor alapj�n
static float  AT_TimeConst;
static float  AT_GainPos;
static float  AT_GainOrient;
static float  AT_Speed;
static float  AT_PoleRE;
static float  AT_PoleIM;
static float  AT_U;
/* SZ�GM�R�S */
#define D_LINE_SENSORS 	   164.0f	// Szenzorsorok t�vols�ga [mm]
#define WIDTH_LINE_SENSOR  185.0f 	// Szenzorsor hossza [mm]
#define LINE_POS_TARTOMANY 128.0f
#define LINE_POS_OFFSET    0    	// 2. szenzorsor elt�r�se az els�h�z k�pest
#define ORIENTATION_CONST  ((D_LINE_SENSORS*LINE_POS_TARTOMANY)/(WIDTH_LINE_SENSOR/2)) // poz�ci��rt�k oszt� konstansa

static int8_t FrontSensorLinePos;
static int8_t RearSensorLinePos;

/* �llapotteres szab�lyoz� a vonalk�vet�sre */
void Do_PositionControl_AT(){
	//ciklusid� ellen�rz�s
	if(TimePositionControl_AT > T_AT ){
		//sz�ml�l� null�z�s a ciklus �jrakezd�s�hez
		TimePositionControl=0;

		AT_Speed = Encoder_GetVelocity();
		D5Percent = 0.5 + 0.5 * AT_Speed;
		if(AT_Speed != 0){
			/* Hibajelek: poz�ci� �s sz�g */
			if(FrontSensor_Data[1] != 0)
				AT_Pos = -(int8_t)FrontSensor_Data[0];//*0.185)/128.0;
			AT_Orient = GetLineOrient();

			AT_T5Percent = D5Percent/AT_Speed;
			AT_TimeConst = (Ksi * AT_T5Percent)/3;		//Ksi-b�l �s T5%-b�l sz�molhat� a T

			/* P�lusok */
			AT_PoleRE = -Ksi/AT_TimeConst;
			AT_PoleIM = (1/AT_TimeConst)*sqrt(1-(Ksi*Ksi));

			AT_GainPos = - (L/(AT_Speed*AT_Speed))*(AT_PoleRE * AT_PoleRE + AT_PoleIM * AT_PoleIM);
			AT_GainOrient = (L/AT_Speed)*(2*AT_PoleRE-AT_GainPos*AT_Speed);

			AT_U = - AT_GainPos*AT_Pos - AT_GainOrient*AT_Orient;

			//tel�t�d�s
			if(AT_U>127)
				AT_U=127;
			if(AT_U<-128)
				AT_U=-128;

			//szervo �ll�t�sa
			SetServoPWMPulse((int8_t)(AT_U));
		}
	}
}

/* PD szab�lyoz� a vonalk�vet�sre */
void Do_PositionControl(){
	//ciklusid� ellen�rz�s �s ha nincs vonal, nincs szab�lyz�s, tartsa az el�z� korm�nysz�get
	if((TimePositionControl > T) && (Get_LineNumber() != NoLine))
	{
		//sz�ml�l� null�z�s a ciklus �jrakezd�s�hez
		TimePositionControl=0;

		//hibajel
		//-128..127
		LinePosition[1]=LinePosition[0]; 				//el�z� hibajel
		LinePosition[0]=(int8_t)FrontSensor_Data[0];	//jelenlegi hibajel

		//PD szab�lyz�
		ServoPosition = -P*LinePosition[0] - D_All*(LinePosition[0]-LinePosition[1]);

		//tel�t�d�s
		if(ServoPosition>127)
			ServoPosition=127;
		if(ServoPosition<-128)
			ServoPosition=-128;

		//szervo �ll�t�sa
		SetServoPWMPulse((int8_t)ServoPosition);
	}
}

/* Get f�ggv�nyek */
/* ServoPosition: -128..127 */
int8_t Get_ServoPosition()
{
	return (int8_t)ServoPosition;
}

/* LinePosition:-128..127 */
int8_t Get_LinePosition()
{
	return (int8_t)FrontSensor_Data[0];
}

/*  */
float Get_P_Corner()
{
	return P_Corner;
}

/*  */
float Get_P_Straight()
{
	return P_Straight;
}

/*  */
float Get_D()
{
	return D_All;
}

/* @brief Vonal orient�ci� lek�rdez�se
 * @retval Sz�ginform�ci� fokban */
float GetLineOrient(){
	float temp;
	if(FrontSensor_Data[1] != 0)
		FrontSensorLinePos = (int8_t)FrontSensor_Data[0];
	if(RearSensor_Data[1] != 0)
		RearSensorLinePos = (int8_t)RearSensor_Data[0] + LINE_POS_OFFSET;
	temp = (FrontSensorLinePos - RearSensorLinePos)/ORIENTATION_CONST;
	return atan(temp)*180/3.1415;
}
