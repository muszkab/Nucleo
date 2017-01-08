/*
 * LineType.c
 *
 *  Created on: 2016. dec. 7.
 *      Author: Muszka
 */

#include "LineType.h"

//vonalt�pus meghat�roz�s ciklusideje
#define T_LINETYPE	2		//T*1ms

//vonalhossz �rt�kek
#define THREELINEDISTANCE_FOLYT_LIMIT 	10	//cm	(szaggatott max 8)
#define ONELINEDISTANCE_FOLYT_LIMIT 	18	//cm	(k�tvonal szaggatottn�l 16cm az egyvonal szaggat�s)
#define DISTANCEDIFFERENCE_MAX			1	//cm	szaggatott h�romvonaln�l, az egy- �s h�romvonal szaggat�s megengedett elt�r�se. N�vleges m�ret: 8cm
#define NOLINEDISTANCE_SZAGG_LIMIT		7	//cm	n�vleges hossz: 8cm
#define TWOLINEDISTANCE_SZAGG_LIMIT_TWOLINE	7//cm	n�vleges hossz: 8cm
#define TWOLINEDISTANCE_SZAGG_LIMIT_ONELINE	15//cm	n�vleges hossz: 16cm

//vonaldarabsz�m sz�r�shez haszn�lt t�mb m�rete
#define ARRAYSIZE	3
//vonaldarabsz�m sz�r�sn�l, mennyi �rt�k lehet k�l�nb�z� a t�mbben, amikor m�g egy�rtelm�nek mondjuk a t�mb�t
//#define TOLERANCE 	2	//nem haszn�ljuk jelenleg

/* V�ltoz�k */
//t�rolja milyen �llapotban vagyunk a vonal alapj�n(darabsz�m; folyt-szagg)
static State_LineType StateLineType = Egyvonal_folyt;
/* Vonalak darabsz�ma a CAN �zenet alapj�n, id�ben ARRAYSIZE darab egym�st k�vet�t t�rolunk el */
static lineType LineNumberArray[ARRAYSIZE];
//adott pillanatban h�ny darab vonalat �ll�tunk
static lineType LineNumber = NoLine;
//el�z� pillanatban h�ny darab vonalat �ll�tottunk
static lineType LineNumberPrev = NoLine;

//milyen hosszan tart egy vonalt�pus
static float NoLineDistance=0;
static float OneLineDistance=0;
static float TwoLineDistance=0;
static float ThreeLineDistance=0;
//vonalt�pusok kezd�pontja az abszol�t t�vols�gban
static float NoLineStartPos=0;
static float OneLineStartPos=0;
static float TwoLineStartPos=0;
static float ThreeLineStartPos=0;

/* Static F�ggv�nyek */
static void NewSensorData();
static void Is_EgyVonal();
static void Is_KetVonal();
static void Is_HaromVonal();
static uint8_t IsElementsEqual(lineType* Array);


/* NewSensorData(): �j �rt�k (FrontSensor_Data[1]) elment�se a t�mbbe (LineNumberArray[])
 * LineNumber: vonaldarabsz�m meghat�roz�s sz�r�ssel: IsElementsEqual(LineNumberArray[])
 * StateLineType: �llapotv�ltoz� be�ll�t�sa: Is_EgyVonal(); Is_KetVonal(); Is_HaromVonal();
 */
void Do_LineType()
{
	//T_GETLINE ciklusid� (T_GETLINE*0.1ms) biztos�t�sa.
	if(TimeLineType > T_LINETYPE)
	{
		//sz�ml�l� null�z�s a ciklus �jrakezd�s�hez
		TimeLineType=0;

		//�j szenzoradat feldolgoz�sa
		NewSensorData();

		//vonalt�pus �llapotv�ltoz�(StateLineType) meghat�roz�s: darabsz�m �s folytonos/szaggatott
		Is_EgyVonal();
		Is_KetVonal();
		Is_HaromVonal();

		//teszt
		if(LineNumber == ThreeLine)
			Led_On(Blue);
		else
			Led_Off(Blue);
	}
}

/* vonalszenzort�l fogadott �j adat(FrontSensor_Data[1]=vonaldarabsz�m) feldolgoz�sa
 * id�beli sz�r�s a vonalt�pusra: IsElementsEqual(LineNumberArray)
 */
static void NewSensorData()
{
	//minden �rt�k eggyel h�tr�bb cs�sztat�sa
	for(int i=0;i<ARRAYSIZE-1;i++)
		LineNumberArray[i+1] = LineNumberArray[i];
	//�j �rt�k
	LineNumberArray[0] = FrontSensor_Data[1];
	//elmentj�k az el�z� �llapot vonaldarabsz�m�t
	LineNumberPrev = LineNumber;
	//ha minden elem egyenl� a t�mbben, ez az �rt�k lesz a vonaldarabsz�m. Ha nem igaz, marad az el�z� vonalt�pus.
	if(IsElementsEqual(LineNumberArray))
		LineNumber = LineNumberArray[0];
}

/* Figyeli, hogy r�g�ta egyvonal van-e, �s ha igen, Egyvonal_folyt-ba �ll�tja az �llapotot
 * Figyeli, hogy szaggatott egyvonal van-e.
 */
static void Is_EgyVonal()
{
	//ha most kezd�d�tt az egyvonalhossz, ezel�tt m�s volt, a kezdeti �rt�k lek�r�se
	if(LineNumberPrev != OneLine && LineNumber == OneLine)
	{
		OneLineStartPos = Encoder_GetDistance_cm();
	}
	//egyvonal hossza jelenleg
	if(LineNumber == OneLine)
	{
		OneLineDistance = Encoder_GetDistance_cm() - OneLineStartPos;
	}
	//hossz ellen�rz�s, ha el�g nagy, berakjuk egyvonal folytonosba
	if(OneLineDistance > ONELINEDISTANCE_FOLYT_LIMIT)
	{
		StateLineType = Egyvonal_folyt;
	}

	//ha elt�nt az egyvonal �s a korm�nysz�g 0 k�r�l van, feltehet�leg egyvonal szaggatott lesz
	if(LineNumber == NoLine && (Get_ServoPosition() <20) &&  (Get_ServoPosition() >-20) && LineNumberPrev == OneLine)
	{
		NoLineStartPos = Encoder_GetDistance_cm();
	}
	//ha ezel�tt nulla vonal volt, �s egy vonal van most, �s kicsi a korm�nysz�g
	if(LineNumberPrev == NoLine && (Get_ServoPosition() <20) &&  (Get_ServoPosition() >-20) && LineNumber == OneLine)
	{
		NoLineDistance = Encoder_GetDistance_cm() - NoLineStartPos;
		//ha egy adott hosszn�l nagyobb a szaggat�s, egyvonal szaggatott lesz az �llapotv�ltoz�
		if(NoLineDistance > NOLINEDISTANCE_SZAGG_LIMIT)
			StateLineType = Egyvonal_szagg;
	}
}

//figyeli hogy k�t vonal van-e, �s szaggatott
static void Is_KetVonal()
{
	//ha egy vonalb�l k�t vonal lett
	if(LineNumber == TwoLine && LineNumberPrev == OneLine)
	{
		//kezd�pont
		TwoLineStartPos = Encoder_GetDistance_cm();
		//els� darab k�t vonal
		if(StateLineType == Egyvonal_folyt)
		{
			//�llapot k�tvonal, de bizonytalan hogy milyen
			StateLineType = Ketvonal_x;
		}
		//m�r k�tvonal_x �llapotban vagyunk, nem az els� k�tvonal
		else if(StateLineType == Ketvonal_x)
		{
			//egyvonal hossz �s k�tvonal hossz ellen�rz�s, hogy szaggatott �llapot-e
			if(OneLineDistance > TWOLINEDISTANCE_SZAGG_LIMIT_ONELINE && TwoLineDistance > TWOLINEDISTANCE_SZAGG_LIMIT_TWOLINE)
			{
				StateLineType = Ketvonal_szagg;
			}
		}
	}
	//ha k�t vonalb�l egy vonal lett
	if(LineNumberPrev == TwoLine && LineNumber == OneLine)
	{
		TwoLineDistance = Encoder_GetDistance_cm() - TwoLineStartPos;
	}
}

//figyeli hogy h�romvonal van-e, �s ha igen, szaggatott vagy folytonos, �s be�ll�tja a StateLineType v�ltoz�t ennek megfelel�en
static void Is_HaromVonal()
{
	//egy vonalb�l h�rom vonal lett
	if(LineNumber==ThreeLine && LineNumberPrev==OneLine)
	{
		//abszol�t t�vols�g kezd�pontja
		ThreeLineStartPos = Encoder_GetDistance_cm();
		//ha normal egyvonal �llapotban  voltunk
		if(StateLineType == Egyvonal_folyt)
		{
			//�llapotot h�romvonalba �ll�tjuk, nem tudjuk folytonos vagy szaggatott
			StateLineType = Haromvonal_x;
		}
		//ha H�romvonal_x �llapotban vagyunk, lehet h szaggatott lesz
		else if(StateLineType == Haromvonal_x)
		{
			//egyvonal hossz��rt m�sik fgv felel�s: Is_EgyVonal()
			//h�rom vonal hossza �s egy vonal hossza �sszehasonl�t�s, ha k�zel egyenl�ek, szaggatott vonal van
			float One_Three_DistanceDifference = OneLineDistance - ThreeLineDistance;
			//ha a k�l�nbs�g a megadott hat�ron bel�l van, szaggatott vonal van
			if(One_Three_DistanceDifference < DISTANCEDIFFERENCE_MAX && One_Three_DistanceDifference > -DISTANCEDIFFERENCE_MAX)
			{
				StateLineType = Haromvonal_szagg;
			}
			//ha nincs hat�ron bel�l, legyen megint normal �llapot
			else
			{
				StateLineType = Egyvonal_folyt;
			}
		}
	}

	//figyelj�k milyen hosszan tart a h�rom vonal hossza
	if(LineNumber == ThreeLine && StateLineType == Haromvonal_x)
	{
		ThreeLineDistance = Encoder_GetDistance_cm() - ThreeLineStartPos;
		//ha el�rte a h�romvonal a megfelel� hosszt, folytonosnak mondjuk
		if(ThreeLineDistance > THREELINEDISTANCE_FOLYT_LIMIT)
		{
			//folytonos h�romvonal van
			StateLineType = Haromvonal_folyt;
		}
	}

	//h�rom vonalb�l egy vonal lett
	if(LineNumber==OneLine && LineNumberPrev==ThreeLine)
	{
		//ha folytonos h�rom vonalnak lett v�ge
		if(StateLineType == Haromvonal_folyt)
		{
			//norm�l egyvonal �llapot
			StateLineType = Egyvonal_folyt;
		}

		//ha v�lhet�en szaggatott vonal van
		else if(StateLineType == Haromvonal_x)
		{
			//abszol�t t�vols�g v�gpontja - kezd�pontja = relat�v t�vols�g
			ThreeLineDistance = Encoder_GetDistance_cm() - ThreeLineStartPos;
			//az egyvonal t�vols�g�t az Is_EgyVonal() sz�molja folyamatosan
		}
	}
}

/* Id�beli sz�r�s a vonalt�pusra, akkor �ll�tjuk biztosra hogy h�ny darab vonal van,
 * ha t�bb egym�s ut�ni �rt�k is egyezik.
 * lineType t�pus� t�mb�t megvizsg�lja, egyenl�-e minden elem,
 * ha igen, 1-gyel t�r vissza, ha hamis, null�val*/
static uint8_t IsElementsEqual(lineType* Array)
{
	//sz�molja h�ny darab elem �rt�ke egyezik meg, ha az �sszes egyezik ( cnt == (ARRASIZE-1) ), egy�rtelm� a vonalt�pus
	uint8_t cnt=0;
	//�rt�kek �sszehasonl�t�sa
	for(int i=0;i<ARRAYSIZE-1;i++)
	{
		if(Array[i] == Array[i+1])
			cnt++;
	}
	//ha minden �rt�k egyezett, visszat�r igazzal
	if(cnt == (ARRAYSIZE-1))
		return 1;
	else
		return 0;
}

/* Get f�ggv�nyek */
lineType Get_LineNumber()
{
	return LineNumber;
}

State_LineType Get_StateLineType()
{
	return StateLineType;
}
