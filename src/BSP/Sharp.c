/*
 * Sharp.c
 *
 *  Created on: 2017. jan. 10.
 *      Author: szabgyul92
 */

/* Includes ------------------------------------------------------------------*/
#include "Sharp.h"

#define BATT_CONST (float)((0.292*256)/3.3) // (<ellenállás osztó érték> * <ADC felbontás>)/3.3V

#define BufferSize 4
uint16_t uhADC1ConvertedValues[BufferSize];

/* Moving Average */
static const float Weight = 0.8;
static float BatteryV_MAvg;
static float FrontSharp_MAvg;
static float RightSharp_MAvg;
static float LeftSharp_MAvg;
/**
  * @brief Initialize and Start all Sharp sensor and Battery Voltage measurement
  * Read the analog values in every 10 ms
  */
void SharpInit(){
	ADC1_Init();
	ADC1_TIM_Init();

	/*## Start the conversion process #######################################*/
	if(HAL_ADC_Start_DMA(&Adc1Handle, (uint32_t*)&uhADC1ConvertedValues, BufferSize) != HAL_OK){
		/* Start Conversation Error */
		Error_Handler();
	}

	/*## TIM counter enable ################################################*/
	if (HAL_TIM_Base_Start(&Adc1_Tim_Handle) != HAL_OK){
		/* Counter Enable Error */
		Error_Handler();
	}
}

/**
  * @brief  Conversion complete callback in non blocking mode
  * @param  AdcHandle : AdcHandle handle
  * @note   This example shows a simple way to report end of conversion, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle){
	LED_Toggle(Yellow);
	BatteryV_MAvg = ExponentialMovingAverage(uhADC1ConvertedValues[0],
			BatteryV_MAvg, Weight);
	FrontSharp_MAvg = ExponentialMovingAverage(uhADC1ConvertedValues[1],
			FrontSharp_MAvg, Weight);
	RightSharp_MAvg = ExponentialMovingAverage(uhADC1ConvertedValues[3],
			RightSharp_MAvg, Weight);
	LeftSharp_MAvg = ExponentialMovingAverage(uhADC1ConvertedValues[2],
			LeftSharp_MAvg, Weight);
}

/*
 * Exponenciális mozgóátlag
 * NewData: új érték
 * CurrentAvg: eddig kiszámolt mozgóátlag
 * Weight: súly: 0..1
 */
float ExponentialMovingAverage(float NewData, float CurrentAvg, float Weight){
	return Weight*NewData+(1-Weight)*CurrentAvg;
}

float MovingAverage(float NewData, float *MovAvBuff, uint8_t lenght){
	int i;
	float MovAv = 0;
	for(i = 0; i < lenght - 1; i++){
		MovAvBuff[i] = MovAvBuff[i+1];
		MovAv += MovAvBuff[i];
	}
	MovAvBuff[lenght - 1] = NewData;
	MovAv += NewData;
	return MovAv/lenght;
}

float GetBatteryVoltage(){
	return BatteryV_MAvg/BATT_CONST;
}

float GetFrontSharpDistance(){
	return 7026*pow(FrontSharp_MAvg,-1.089);
}
float GetRightSharpDistance(){
	return -9.174*log(0.003454*RightSharp_MAvg);
}
float GetLeftSharpDistance(){
	return 10.3*log(275.08/LeftSharp_MAvg);
}


/********** Akadályfelismerés **********/
//sharp-sharp: 23cm
//vonal-sharp: 10.5cm
#define GATEDISTANCE		25	//vonal-kapu: 31cm; milyen messze van maximum a kapu a sharp szenzortól
#define WALLDISTANCE		22	//vonal-fal: 30cm; milyen messze van maximum a fal a sharp szenzortól
#define BORDADISTANCE		18	//vonal-borda: 26cm; milyen messze van maximum a borda a sharp szenzortól
#define KORFORGALOMDISTANCE	22	//vonal-körforgalom: 30cm; milyen messze van maximum a körforgalom a sharp szenzortól
#define NOWALLDISTANCE		30	//ha ennél nagyobb távolságot mérünk, akkor nincs fal
#define WALLLENGTH			50	//névleges: 60cm
#define SIMA_BORDA_SZORASHATAR	2	//sima: 0cm; bordas: 4cm a távolság szórása

//jobb oldali sharp által mért távolság
static float RightSharpDistance=NOWALLDISTANCE+10;
//állapotváltozó, mely mutatja milyen fal van globálisan az autó mellett
static State_Wall StateWall_Global=Nincsfal;
//állapotváltozó, mely mutatja milyen fal van jobb oldalt az autó mellett
static State_Wall StateWall_Right=Nincsfal;
//állapotváltozó, mely mutatja milyen fal van bal oldalt az autó mellett
static State_Wall StateWall_Left=Nincsfal;
//milyen távol volt a jobboldali fal minimum
static float RightWall_Distance_Min=NOWALLDISTANCE+10;
//milyen távol volt a jobboldali fal maximum
static float RightWall_Distance_Max=0;
//milyen hosszan tart a jobboldali fal
static float RightWall_Length=0;
//hosszméréshez mindig eltároljuk az elõzõ pillanatbeli abszolút megtett utat, ezt vonjuk ki a jelenlegi abszolút útból
static float RightWall_Length_AbsolutePos=0;

void Is_Kapu()
{
	if(StateWall_Global == Nincsfal)
	{
		//ha mindkét szenzor érzékeli a kapu szélét
		if((GetRightSharpDistance() < GATEDISTANCE) && GetLeftSharpDistance() < GATEDISTANCE)
		{
			StateWall_Global = Kapu;
		}
	}
}

void Is_Simafal()
{
	if(StateWall_Global == Nincsfal)
	{
		RightSharpDistance = GetRightSharpDistance();
		//ha nem jelez akadályt, az állapotot nincsfalra állítjuk
		if(RightSharpDistance > WALLDISTANCE)
		{
			StateWall_Right = Nincsfal;
		}
		//ha a jobb oldali szenzor akadályt jelez
		else if(RightSharpDistance < WALLDISTANCE)
		{
			//ha most elõször jelez akadályt
			if(StateWall_Right == Nincsfal)
			{
				RightWall_Distance_Min = RightWall_Distance_Max = RightSharpDistance;
				RightWall_Length_AbsolutePos = Encoder_GetDistance_cm();
				StateWall_Right = Fal_jobb;
			}
			//ha már jobboldalt fal állapot volt
			else if(StateWall_Right == Fal_jobb)
			{
				//minimum távolság
				if(RightSharpDistance<RightWall_Distance_Min)
					RightWall_Distance_Min = RightSharpDistance;
				//maximum távolság
				if(RightSharpDistance > RightWall_Distance_Max)
					RightWall_Distance_Max = RightSharpDistance;

				//fal hossza: hozzáadjuk az egy cikkus alatt megtett kis útszakaszt, majd lementjük az elõzõ abszolút utat
				RightWall_Length += Encoder_GetDistance_cm() - RightWall_Length_AbsolutePos;
				RightWall_Length_AbsolutePos = Encoder_GetDistance_cm();

				//leellenõrizzük a fal hosszát, hogy elég hosszú-e már
				if(RightWall_Length > WALLLENGTH)
				{
					//megnézzük mennyivel tér el a min és max távolság érték, ha kicsivel sima fal, ha sokkal bordás fal
					if((RightWall_Distance_Max - RightWall_Distance_Min) < SIMA_BORDA_SZORASHATAR)
					{
						StateWall_Right = Simafal_jobb;
					}
					else
					{
						StateWall_Right = Bordasfal_jobb;
					}
				}
			}
		}
		//ha a bal oldali szenzor akadályt jelez, és a jobb oldali nem jelez
		if(GetLeftSharpDistance() < WALLDISTANCE && GetRightSharpDistance() > NOWALLDISTANCE)
		{
			StateWall_Global = Simafal_bal;
		}
	}
}

void Is_Bordasfal()
{
	if(StateWall_Global == Nincsfal)
	{
		//ha a jobb oldali szenzor akadályt jelez
		if(GetRightSharpDistance() < BORDADISTANCE)
		{
			StateWall_Global = Bordasfal_jobb;
		}
		//ha a bal oldali szenzor akadályt jelez
		if(GetLeftSharpDistance() < BORDADISTANCE)
		{
			StateWall_Global = Bordasfal_bal;
		}
	}
}

void Is_Korforgalom()
{
	//TODO: lehet hogy hamarabb visszavált normál egyvonal folytonosba!
	if(Get_StateLineType() == Egyvonal_szagg && StateWall_Global == Nincsfal)
	{
		//ha a jobb oldali szenzor akadályt jelez
		if(GetRightSharpDistance() < KORFORGALOMDISTANCE)
		{
			StateWall_Global = Korforgalom_jobb;
		}
		//ha a bal oldali szenzor akadályt jelez
		if(GetLeftSharpDistance() < KORFORGALOMDISTANCE)
		{
			StateWall_Global = Korforgalom_bal;
		}
	}
}
