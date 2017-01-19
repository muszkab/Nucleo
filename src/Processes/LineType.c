/*
 * LineType.c
 *
 *  Created on: 2016. dec. 7.
 *      Author: Muszka
 */

#include "LineType.h"

//vonaltípus meghatározás ciklusideje
#define T_LINETYPE	2		//T*1ms

//vonalhossz értékek
#define THREELINEDISTANCE_FOLYT_LIMIT 	10	//cm	(szaggatott max 8)
#define ONELINEDISTANCE_FOLYT_LIMIT 	18	//cm	(kétvonal szaggatottnál 16cm az egyvonal szaggatás)
#define DISTANCEDIFFERENCE_MAX			1	//cm	szaggatott háromvonalnál, az egy- és háromvonal szaggatás megengedett eltérése. Névleges méret: 8cm
#define NOLINEDISTANCE_SZAGG_MIN		7	//cm	névleges hossz: 8cm
#define NOLINEDISTANCE_SZAGG_MAX		9	//cm	névleges hossz: 8cm
#define TWOLINEDISTANCE_SZAGG_LIMIT_TWOLINE	6//cm	névleges hossz: 8cm
#define TWOLINEDISTANCE_SZAGG_LIMIT_ONELINE	13//cm	névleges hossz: 16cm

//vonaldarabszám szûréshez használt tömb mérete
#define ARRAYSIZE	3
//vonaldarabszám szûrésnél, mennyi érték lehet különbözõ a tömbben, amikor még egyértelmûnek mondjuk a tömböt
//#define TOLERANCE 	2	//nem használjuk jelenleg

/* Változók */
//tárolja milyen állapotban vagyunk a vonal alapján(darabszám; folyt-szagg)
static State_LineType StateLineType = Egyvonal_folyt;
/* Vonalak darabszáma a CAN üzenet alapján, idõben ARRAYSIZE darab egymást követõt tárolunk el */
static lineType LineNumberArray[ARRAYSIZE];
//adott pillanatban hány darab vonalat állítunk
static lineType LineNumber = NoLine;
//elõzõ pillanatban hány darab vonalat állítottunk
static lineType LineNumberPrev = NoLine;

//milyen hosszan tart egy vonaltípus
static float NoLineDistance=0;
static float OneLineDistance=0;
static float TwoLineDistance=0;
static float ThreeLineDistance=0;
//vonaltípusok kezdõpontja az abszolút távolságban
static float NoLineStartPos=0;
static float OneLineStartPos=0;
static float TwoLineStartPos=0;
static float ThreeLineStartPos=0;

/* Static Függvények */
static void NewSensorData();
static void Is_EgyVonal();
static void Is_KetVonal();
static void Is_HaromVonal();
static uint8_t IsElementsEqual(lineType* Array);


/* NewSensorData(): új érték (FrontSensor_Data[1]) elmentése a tömbbe (LineNumberArray[])
 * LineNumber: vonaldarabszám meghatározás szûréssel: IsElementsEqual(LineNumberArray[])
 * StateLineType: állapotváltozó beállítása: Is_EgyVonal(); Is_KetVonal(); Is_HaromVonal();
 */
void Do_LineType()
{
	//T_GETLINE ciklusidõ (T_GETLINE*0.1ms) biztosítása.
	if(TimeLineType > T_LINETYPE)
	{
		//számláló nullázás a ciklus újrakezdéséhez
		TimeLineType=0;

		//új szenzoradat feldolgozása
		NewSensorData();

		//vonaltípus állapotváltozó(StateLineType) meghatározás: darabszám és folytonos/szaggatott vagy keresztvonal
		Is_EgyVonal();
		Is_KetVonal();
		Is_HaromVonal();
		if(LineNumber == Full)
		{
			StateLineType = Keresztvonal;
		}

		//teszt
		if(LineNumber == ThreeLine)
			Led_On(Blue);
		else
			Led_Off(Blue);

		if(LineNumber == Full)
			Led_On(Red);
		else
			Led_Off(Red);
	}
}

/* vonalszenzortól fogadott új adat(FrontSensor_Data[1]=vonaldarabszám) feldolgozása
 * idõbeli szûrés a vonaltípusra: IsElementsEqual(LineNumberArray)
 */
static void NewSensorData()
{
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
}

/* Figyeli, hogy régóta egyvonal van-e, és ha igen, Egyvonal_folyt-ba állítja az állapotot
 * Figyeli, hogy szaggatott egyvonal van-e.
 */
static void Is_EgyVonal()
{
	//ha most kezdõdött az egyvonalhossz, ezelõtt más volt, a kezdeti érték lekérése
	if(LineNumberPrev != OneLine && LineNumber == OneLine)
	{
		OneLineStartPos = Encoder_GetDistance_cm();
	}

	//egyvonal hossza jelenleg
	if(LineNumber == OneLine)
	{
		OneLineDistance = Encoder_GetDistance_cm() - OneLineStartPos;
	}

	//hossz ellenõrzés, ha elég nagy, és elõtte más állapotban  volt, berakjuk egyvonal folytonosba
	if(StateLineType != Egyvonal_folyt && LineNumber == OneLine && OneLineDistance > ONELINEDISTANCE_FOLYT_LIMIT)
	{
		StateLineType = Egyvonal_folyt;
		SendDebugMessage_Text("Egyvonal folytonos");

	}

	//ha eltûnt az egyvonal és a kormányszög 0 körül van, feltehetõleg egyvonal szaggatott lesz
	if(LineNumber == NoLine && (Get_ServoPosition() <20) &&  (Get_ServoPosition() >-20) && LineNumberPrev == OneLine)
	{
		NoLineStartPos = Encoder_GetDistance_cm();
	}
	//ha ezelõtt nulla vonal volt, és egy vonal van most, és kicsi a kormányszög
	if(LineNumberPrev == NoLine && (Get_ServoPosition() <20) &&  (Get_ServoPosition() >-20) && LineNumber == OneLine)
	{
		NoLineDistance = Encoder_GetDistance_cm() - NoLineStartPos;
		//ha normál egyvonal folytonos állapotban volt és egy adott tartomyányon belül van a szaggatás, egyvonal szaggatott lesz az állapotváltozó
		if(StateLineType == Egyvonal_folyt && NoLineDistance > NOLINEDISTANCE_SZAGG_MIN && NoLineDistance < NOLINEDISTANCE_SZAGG_MAX)
		{
			StateLineType = Egyvonal_szagg;
			SendDebugMessage_Text("Egyvonal szaggatott");

		}
	}
}
//TODO átgondolni!
//figyeli hogy két vonal van-e, és szaggatott
static void Is_KetVonal()
{
	//ha valamilyen vonalból két vonal lett
	if(LineNumber == TwoLine && LineNumberPrev != TwoLine)
	{
		//kezdõpont
		TwoLineStartPos = Encoder_GetDistance_cm();
		//elsõ darab két vonal
		if(StateLineType == Egyvonal_folyt)
		{
			//állapot kétvonal, de bizonytalan hogy milyen
			StateLineType = Ketvonal_x;
			SendDebugMessage_Text("Ketvonal x");
		}
	}

	//már kétvonal_x állapotban vagyunk és egy vonal van
	if(StateLineType == Ketvonal_x && LineNumber == OneLine)
	{
		//egyvonal hossz és kétvonal hossz ellenõrzés, hogy szaggatott állapot-e
		if(OneLineDistance > TWOLINEDISTANCE_SZAGG_LIMIT_ONELINE && TwoLineDistance > TWOLINEDISTANCE_SZAGG_LIMIT_TWOLINE)
		{
			StateLineType = Ketvonal_szagg;
			SendDebugMessage_Text("Ketvonal szaggatott");
		}
	}

	//ha két vonalból másmilyen vonal lett
	if(LineNumberPrev == TwoLine && LineNumber != TwoLine)
	{
		TwoLineDistance = Encoder_GetDistance_cm() - TwoLineStartPos;
	}
}

//figyeli hogy háromvonal van-e, és ha igen, szaggatott vagy folytonos, és beállítja a StateLineType változót ennek megfelelõen
static void Is_HaromVonal()
{
	//nem három(egy vagy kettõ) vonalból három vonal lett
	if(LineNumber == ThreeLine && LineNumberPrev != ThreeLine)
	{
		//abszolút távolság kezdõpontja
		ThreeLineStartPos = Encoder_GetDistance_cm();
		//ha normal egyvonal állapotban  voltunk vagy Ketvonal_x-ben, mert lehet hogy elsõzör csak két vonalat érzékel a háromból
		if((StateLineType == Egyvonal_folyt) || (StateLineType == Ketvonal_x))
		{
			//állapotot háromvonalba állítjuk, nem tudjuk folytonos vagy szaggatott
			StateLineType = Haromvonal_x;
			SendDebugMessage_Text("Haromvonal x");
		}
		//ha Háromvonal_x állapotban vagyunk, lehet h szaggatott lesz
		else if(StateLineType == Haromvonal_x)
		{
			//egyvonal hosszáért másik fgv felelõs: Is_EgyVonal()
			//három vonal hossza és egy vonal hossza összehasonlítás, ha közel egyenlõek, szaggatott vonal van
			float One_Three_DistanceDifference = OneLineDistance - ThreeLineDistance;
			//ha a különbség a megadott határon belül van, szaggatott vonal van
			if(One_Three_DistanceDifference < DISTANCEDIFFERENCE_MAX && One_Three_DistanceDifference > -DISTANCEDIFFERENCE_MAX)
			{
				StateLineType = Haromvonal_szagg;
				SendDebugMessage_Text("Haromvonal szaggatott");
			}
		}
	}

	//figyeljük milyen hosszan tart a három vonal hossza
	if(LineNumber == ThreeLine && StateLineType == Haromvonal_x)
	{
		ThreeLineDistance = Encoder_GetDistance_cm() - ThreeLineStartPos;
		//ha elérte a háromvonal a megfelelõ hosszt, folytonosnak mondjuk
		if(ThreeLineDistance > THREELINEDISTANCE_FOLYT_LIMIT)
		{
			//folytonos háromvonal van
			StateLineType = Haromvonal_folyt;
			SendDebugMessage_Text("Haromvonal folytonos");
		}
	}

	//ha vélhetõen szaggatott vonal van és három vonalból valami más vonal lett
	if(StateLineType == Haromvonal_x && LineNumber != ThreeLine && LineNumberPrev == ThreeLine)
	{
		//abszolút távolság végpontja - kezdõpontja = relatív távolság
		ThreeLineDistance = Encoder_GetDistance_cm() - ThreeLineStartPos;
		//az egyvonal távolságát az Is_EgyVonal() számolja folyamatosan
	}
}

/* Idõbeli szûrés a vonaltípusra, akkor állítjuk biztosra hogy hány darab vonal van,
 * ha több egymás utáni érték is egyezik.
 * lineType típusú tömböt megvizsgálja, egyenlõ-e minden elem,
 * ha igen, 1-gyel tér vissza, ha hamis, nullával*/
static uint8_t IsElementsEqual(lineType* Array)
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

/* Get függvények */
lineType Get_LineNumber()
{
	return LineNumber;
}

State_LineType Get_StateLineType()
{
	return StateLineType;
}

float Get_ThreeLineDistance()
{
	return ThreeLineDistance;
}

/* Set függvények */
void Set_StateLineType(State_LineType newStateLineType)
{
	StateLineType = newStateLineType;
}
