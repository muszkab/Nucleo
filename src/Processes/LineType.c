/*
 * LineType.c
 *
 *  Created on: 2016. dec. 7.
 *      Author: Muszka
 */

#include "LineType.h"

//vonaltípus meghatározás ciklusideje
#define T_LINETYPE	2		//T*1ms

//hány másodpercig tartson egy állapotot biztosan
#define SECONDLIMIT	2
//kanyart jelzõ három folytonos vonal érzékelési ideje
#define ContinousMinTime_folyt	6/T_LINETYPE	//ContinousMinTime*1ms
#define ContinousMinTime_szagg	10/T_LINETYPE
#define CornerSpeedHigh_Time	1000/T_LINETYPE	//1 másodperc

//vonaldarabszám szûréshez használt tömb mérete
#define ARRAYSIZE	3
//vonaldarabszám szûrésnél, mennyi érték lehet különbözõ a tömbben, amikor még egyértelmûnek mondjuk a tömböt
#define TOLERANCE 	2	//nem használjuk jelenleg

/* Változók */
/* Vonalak darabszáma a CAN üzenet alapján, idõben ARRAYSIZE darab egymást követõt tárolunk el */
lineType LineNumberArray[ARRAYSIZE];
//adott pillanatban hány darab vonalat állítunk
lineType LineNumber = NoLine;
//elõzõ pillanatban hány darab vonalat állítottunk
lineType LineNumberPrev = NoLine;

//TODO: törlés!
//számolja mennyi ideig van egy darab vonal
uint32_t OneLineTime = 0;
//számolja mennyi ideig van három darab vonal
uint32_t ThreeLineTime = 0;

/* új érték (FrontSensor_Data[1]) elmentése a tömbbe (LineNumberArray[])
 * idõbeli szûrés a vonaltípusra: IsElementsEqual(LineNumberArray)
 *
 * számolja az idõt, amíg folyamatosan egy vagy három vonal van: OneLineTime, ThreeLineTime
 * állítja a State változót: SetSpeedState();
 */
void Do_GetLineType()
{
	//T_GETLINE ciklusidõ (T_GETLINE*0.1ms) biztosítása.
	if(TimeLineType > T_LINETYPE)
	{
		//számláló nullázás a ciklus újrakezdéséhez
		TimeLineType=0;

		//minden érték eggyel hátrébb csúsztatása
		for(int i=0;i<ARRAYSIZE-1;i++)
			LineNumberArray[i+1] = LineNumberArray[i];
		//új érték
		LineNumberArray[0] = FrontSensor_Data[1];
		//elmentjük az elõzõ állapot vonaldarabszámát
		LineNumberPrev = LineNumber;
		//ha minden elem egyenlõ a tömbben, ez az érték lesz a vonaldarabszám. Ha nem igaz, marad az elõzõ vonaltípus.
		if(IsElementsEqual(LineNumberArray))
			LineNumber = LineNumberArray[0];

		//egy vonalból három vonal lett
		if(LineNumber==ThreeLine && LineNumberPrev==OneLine)
		{

		}

		//három vonalból egy vonal lett
		if(LineNumber==OneLine && LineNumberPrev==ThreeLine)
		{

		}

		//törlés
		if(LineNumber == OneLine)
		{
			//T_GETLINE*1ms egységekben
			OneLineTime++;
			ThreeLineTime=0;
		}
		if(LineNumber == ThreeLine)
		{
			//T_GETLINE*1ms egységekben
			ThreeLineTime++;
			//OneLineTime=0;
		}

		//sebességállapot állítás
		SetSpeedState();

		//teszt
		if(LineNumber == ThreeLine)
			Led_On(Blue);
		else
			Led_Off(Blue);

		//változó érték beállítás az üzenettömbben(Debugszoftver)
		//TODO: máshol kéne meghívni?
		//SetValue_AtMessageArray(var_LineNumber, (float)LineNumber);
	}
}

/* Beállítja a sebesség állapotot az elõzõ állapot, és a vonaltípus alapján: egyenes vagy kanyar */
void SetSpeedState()
{
	//másodperces számláló
	static uint8_t second=0;
	//TimeSpeedState:1ms, second:1000*1ms=1s
	if(TimeSpeedState > 1000)
	{
		TimeSpeedState=0;
		second++;
	}

	/* Ha eltelt SECONDLIMIT másodperc az elõzõ állopváltás óta,
	 * és egy adott ideje(ContinousMinTime) három darab vonal van,
	 * csak akkor válthat újból állapotot.
	 */
	if((second>SECONDLIMIT) )
	{
		/* egyenesbõl váltson kanyarra */
		if(StateQ1==Straight && (ThreeLineTime > ContinousMinTime_folyt))
		{
			StateQ1 = CornerIn;
			second=0;

			OneLineTime=0;
			ThreeLineTime=0;
			//teszt
			//Led_On(Red);
		}
		else
		{
			/* kanyarból váltson egyenesbe */
			if((StateQ1==CornerOut || StateQ1==CornerIn) && (ThreeLineTime > ContinousMinTime_szagg))
			{
				StateQ1 = Straight;
				second=0;
				//állapotváltásnál nullázzuk a vonalidõket
				OneLineTime=0;
				ThreeLineTime=0;

				//teszt
				//Led_Off(Red);
			}
		}
	}

	//kanyarbejáratból váltson kijáratba
	if(StateQ1==CornerIn && (OneLineTime > CornerSpeedHigh_Time))
	{
		StateQ1=CornerOut;
		//second=0;

		OneLineTime=0;
		ThreeLineTime=0;
	}

	//teszt
/*	if(second>SECONDLIMIT)
		Led_On(Red);
	else
		Led_Off(Red);
*/
}

/* Idõbeli szûrés a vonaltípusra, akkor állítjuk biztosra hogy hány darab vonal van,
 * ha több egymás utáni érték is egyezik.
 * lineType típusú tömböt megvizsgálja, egyenlõ-e minden elem,
 * ha igen, 1-gyel tér vissza, ha hamis, nullával*/
uint8_t IsElementsEqual(lineType* Array)
{
	//számolja hány darab elem értéke egyezik meg, ha az összes egyezik ( cnt == (ARRASIZE-1) ), egyértelmû a vonaltípus
	uint8_t cnt=0;
	//értékek összehasonlítása
	for(int i=0;i<ARRAYSIZE-1;i++)
	{
		if(Array[i] == Array[i+1])
			cnt++;
	}
	//ha minden érték egyezett, visszatér igazzal
	if(cnt == (ARRAYSIZE-1))
		return 1;
	else
		return 0;
}
