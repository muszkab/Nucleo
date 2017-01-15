/*
 * Sharp.c
 *
 *  Created on: 2017. jan. 10.
 *      Author: szabgyul92
 */

/* Includes ------------------------------------------------------------------*/
#include "Sharp.h"

#define BATT_CONST (float)((0.292*256)/3.3) // (<ellen�ll�s oszt� �rt�k> * <ADC felbont�s>)/3.3V

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
 * Exponenci�lis mozg��tlag
 * NewData: �j �rt�k
 * CurrentAvg: eddig kisz�molt mozg��tlag
 * Weight: s�ly: 0..1
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


/********** Akad�lyfelismer�s **********/
//sharp-sharp: 23cm
//vonal-sharp: 10.5cm
#define GATEDISTANCE		25	//vonal-kapu: 31cm; milyen messze van maximum a kapu a sharp szenzort�l
#define WALLDISTANCE		22	//vonal-fal: 30cm; milyen messze van maximum a fal a sharp szenzort�l
#define BORDADISTANCE		18	//vonal-borda: 26cm; milyen messze van maximum a borda a sharp szenzort�l
#define KORFORGALOMDISTANCE	22	//vonal-k�rforgalom: 30cm; milyen messze van maximum a k�rforgalom a sharp szenzort�l
#define NOWALLDISTANCE		30	//ha enn�l nagyobb t�vols�got m�r�nk, akkor nincs fal
#define WALLLENGTH			50	//n�vleges: 60cm
#define SIMA_BORDA_SZORASHATAR	2	//sima: 0cm; bordas: 4cm a t�vols�g sz�r�sa

//jobb oldali sharp �ltal m�rt t�vols�g
static float RightSharpDistance=NOWALLDISTANCE+10;
//�llapotv�ltoz�, mely mutatja milyen fal van glob�lisan az aut� mellett
static State_Wall StateWall_Global=Nincsfal;
//�llapotv�ltoz�, mely mutatja milyen fal van jobb oldalt az aut� mellett
static State_Wall StateWall_Right=Nincsfal;
//�llapotv�ltoz�, mely mutatja milyen fal van bal oldalt az aut� mellett
static State_Wall StateWall_Left=Nincsfal;
//milyen t�vol volt a jobboldali fal minimum
static float RightWall_Distance_Min=NOWALLDISTANCE+10;
//milyen t�vol volt a jobboldali fal maximum
static float RightWall_Distance_Max=0;
//milyen hosszan tart a jobboldali fal
static float RightWall_Length=0;
//hosszm�r�shez mindig elt�roljuk az el�z� pillanatbeli abszol�t megtett utat, ezt vonjuk ki a jelenlegi abszol�t �tb�l
static float RightWall_Length_AbsolutePos=0;

void Is_Kapu()
{
	if(StateWall_Global == Nincsfal)
	{
		//ha mindk�t szenzor �rz�keli a kapu sz�l�t
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
		//ha nem jelez akad�lyt, az �llapotot nincsfalra �ll�tjuk
		if(RightSharpDistance > WALLDISTANCE)
		{
			StateWall_Right = Nincsfal;
		}
		//ha a jobb oldali szenzor akad�lyt jelez
		else if(RightSharpDistance < WALLDISTANCE)
		{
			//ha most el�sz�r jelez akad�lyt
			if(StateWall_Right == Nincsfal)
			{
				RightWall_Distance_Min = RightWall_Distance_Max = RightSharpDistance;
				RightWall_Length_AbsolutePos = Encoder_GetDistance_cm();
				StateWall_Right = Fal_jobb;
			}
			//ha m�r jobboldalt fal �llapot volt
			else if(StateWall_Right == Fal_jobb)
			{
				//minimum t�vols�g
				if(RightSharpDistance<RightWall_Distance_Min)
					RightWall_Distance_Min = RightSharpDistance;
				//maximum t�vols�g
				if(RightSharpDistance > RightWall_Distance_Max)
					RightWall_Distance_Max = RightSharpDistance;

				//fal hossza: hozz�adjuk az egy cikkus alatt megtett kis �tszakaszt, majd lementj�k az el�z� abszol�t utat
				RightWall_Length += Encoder_GetDistance_cm() - RightWall_Length_AbsolutePos;
				RightWall_Length_AbsolutePos = Encoder_GetDistance_cm();

				//leellen�rizz�k a fal hossz�t, hogy el�g hossz�-e m�r
				if(RightWall_Length > WALLLENGTH)
				{
					//megn�zz�k mennyivel t�r el a min �s max t�vols�g �rt�k, ha kicsivel sima fal, ha sokkal bord�s fal
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
		//ha a bal oldali szenzor akad�lyt jelez, �s a jobb oldali nem jelez
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
		//ha a jobb oldali szenzor akad�lyt jelez
		if(GetRightSharpDistance() < BORDADISTANCE)
		{
			StateWall_Global = Bordasfal_jobb;
		}
		//ha a bal oldali szenzor akad�lyt jelez
		if(GetLeftSharpDistance() < BORDADISTANCE)
		{
			StateWall_Global = Bordasfal_bal;
		}
	}
}

void Is_Korforgalom()
{
	//TODO: lehet hogy hamarabb visszav�lt norm�l egyvonal folytonosba!
	if(Get_StateLineType() == Egyvonal_szagg && StateWall_Global == Nincsfal)
	{
		//ha a jobb oldali szenzor akad�lyt jelez
		if(GetRightSharpDistance() < KORFORGALOMDISTANCE)
		{
			StateWall_Global = Korforgalom_jobb;
		}
		//ha a bal oldali szenzor akad�lyt jelez
		if(GetLeftSharpDistance() < KORFORGALOMDISTANCE)
		{
			StateWall_Global = Korforgalom_bal;
		}
	}
}
