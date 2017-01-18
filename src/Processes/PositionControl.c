/*
 * PositionControl.c
 *
 *  Created on: 2016. dec. 1.
 *      Author: Muszka
 */

#include "PositionControl.h"
#include "math.h"

//szabályzó paraméterek
#define	P_CORNER	1.5f
#define	P_STRAIGHT	1.0f
#define D			20.0f
#define T			10		//T*1ms

/* static változók */
//beavatkozó jel
static float ServoPosition=0;
//P értéke attól függõen milyen állapotban vagyunk: kanyarban magassabb, egyenesben kisebb P
static float P			= P_CORNER;
static float P_Corner	= P_CORNER;
static float P_Straight	= P_STRAIGHT;
//D tag, jelenleg ugyanaz az érték mindenhol
static float D_All		= D;
//hibajel, mostani[0] és elõzõ[1] érték
static int8_t LinePosition[2];

/*************************************************/
/************ ÁLLAPOTTERES SZABÁLYZÓ**************/
/*************************************************/
//TODO Pozíció -128..127, szög -60..60, szervo -128..127. Szög nincs azonos tartományban, meg kell oldani
#define 	 Ksi		0.9		// Csillapítási tényezõ, ajánlott értéke : 0,7 0,9
#define 	 T_AT 		10 		// Ciklusidõ [ms]
#define		 L			0.385f	// Hátsó kerék - szenzorsor távolság [m]
static float  D5Percent;	// Beállási úthossz [m]
static float  AT_T5Percent;			// Beállási úthosszhoz tartozó idõ							 [m/s]
static float  AT_Orient;			// orientáció
static float  AT_Pos;				// pozíció a Front vonalszenzor alapján
static float  AT_TimeConst;
static float  AT_GainPos;
static float  AT_GainOrient;
static float  AT_Speed;
static float  AT_PoleRE;
static float  AT_PoleIM;
static float  AT_U;
/* SZÖGMÉRÉS */
#define D_LINE_SENSORS 	   164.0f	// Szenzorsorok távolsága [mm]
#define WIDTH_LINE_SENSOR  185.0f 	// Szenzorsor hossza [mm]
#define LINE_POS_TARTOMANY 128.0f
#define LINE_POS_OFFSET    0    	// 2. szenzorsor eltérése az elsõhöz képest
#define ORIENTATION_CONST  ((D_LINE_SENSORS*LINE_POS_TARTOMANY)/(WIDTH_LINE_SENSOR/2)) // pozícióérték osztó konstansa

static int8_t FrontSensorLinePos;
static int8_t RearSensorLinePos;

/* Állapotteres szabályozó a vonalkövetésre */
void Do_PositionControl_AT(){
	//ciklusidõ ellenõrzés
	if(TimePositionControl_AT > T_AT ){
		//számláló nullázás a ciklus újrakezdéséhez
		TimePositionControl=0;

		AT_Speed = Encoder_GetVelocity();
		D5Percent = 0.5 + 0.5 * AT_Speed;
		if(AT_Speed != 0){
			/* Hibajelek: pozíció és szög */
			if(FrontSensor_Data[1] != 0)
				AT_Pos = -(int8_t)FrontSensor_Data[0];//*0.185)/128.0;
			AT_Orient = GetLineOrient();

			AT_T5Percent = D5Percent/AT_Speed;
			AT_TimeConst = (Ksi * AT_T5Percent)/3;		//Ksi-bõl és T5%-ból számolható a T

			/* Pólusok */
			AT_PoleRE = -Ksi/AT_TimeConst;
			AT_PoleIM = (1/AT_TimeConst)*sqrt(1-(Ksi*Ksi));

			AT_GainPos = - (L/(AT_Speed*AT_Speed))*(AT_PoleRE * AT_PoleRE + AT_PoleIM * AT_PoleIM);
			AT_GainOrient = (L/AT_Speed)*(2*AT_PoleRE-AT_GainPos*AT_Speed);

			AT_U = - AT_GainPos*AT_Pos - AT_GainOrient*AT_Orient;

			//telítõdés
			if(AT_U>127)
				AT_U=127;
			if(AT_U<-128)
				AT_U=-128;

			//szervo állítása
			SetServoPWMPulse((int8_t)(AT_U));
		}
	}
}

/* PD szabályozó a vonalkövetésre */
void Do_PositionControl(){
	//ciklusidõ ellenõrzés és ha nincs vonal, nincs szabályzás, tartsa az elõzõ kormányszöget
	if((TimePositionControl > T) && (Get_LineNumber() != NoLine))
	{
		//számláló nullázás a ciklus újrakezdéséhez
		TimePositionControl=0;

		//hibajel
		//-128..127
		LinePosition[1]=LinePosition[0]; 				//elõzõ hibajel
		LinePosition[0]=(int8_t)FrontSensor_Data[0];	//jelenlegi hibajel

		//PD szabályzó
		ServoPosition = -P*LinePosition[0] - D_All*(LinePosition[0]-LinePosition[1]);

		//telítõdés
		if(ServoPosition>127)
			ServoPosition=127;
		if(ServoPosition<-128)
			ServoPosition=-128;

		//szervo állítása
		SetServoPWMPulse((int8_t)ServoPosition);
	}
}

/* Get függvények */
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

/* @brief Vonal orientáció lekérdezése
 * @retval Szöginformáció fokban */
float GetLineOrient(){
	float temp;
	if(FrontSensor_Data[1] != 0)
		FrontSensorLinePos = (int8_t)FrontSensor_Data[0];
	if(RearSensor_Data[1] != 0)
		RearSensorLinePos = (int8_t)RearSensor_Data[0] + LINE_POS_OFFSET;
	temp = (FrontSensorLinePos - RearSensorLinePos)/ORIENTATION_CONST;
	return atan(temp)*180/3.1415;
}
