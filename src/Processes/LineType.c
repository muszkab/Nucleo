/*
 * LineType.c
 *
 *  Created on: 2016. dec. 7.
 *      Author: Muszka
 */

#include "LineType.h"

//vonalt�pus meghat�roz�s ciklusideje
#define T_LINETYPE	2		//T*1ms

//h�ny m�sodpercig tartson egy �llapotot biztosan
#define SECONDLIMIT	2
//kanyart jelz� h�rom folytonos vonal �rz�kel�si ideje
#define ContinousMinTime_folyt	6/T_LINETYPE	//ContinousMinTime*1ms
#define ContinousMinTime_szagg	10/T_LINETYPE
#define CornerSpeedHigh_Time	1000/T_LINETYPE	//1 m�sodperc

//vonalhossz �rt�kek
#define THREELINEDISTANCE_FOLYT_LIMIT 	10	//?
#define ONELINEDISTANCE_FOLYT_LIMIT 	10	//?
#define DISTANCEDIFFERENCE_MAX			10	//?

//vonaldarabsz�m sz�r�shez haszn�lt t�mb m�rete
#define ARRAYSIZE	3
//vonaldarabsz�m sz�r�sn�l, mennyi �rt�k lehet k�l�nb�z� a t�mbben, amikor m�g egy�rtelm�nek mondjuk a t�mb�t
#define TOLERANCE 	2	//nem haszn�ljuk jelenleg

typedef enum{
	Egyvonal_folyt = 0,
	Egyvonal_szagg,
	Ketvonal_folyt,
	Ketvonal_szagg,
	Haromvonal_x,
	Haromvonal_folyt,
	Haromvonal_szagg
}State_LineType;

/* V�ltoz�k */
//t�rolja milyen �llapotban vagyunk a vonal alapj�n(darabsz�m; folyt-szagg)
State_LineType StateLineType = Egyvonal_folyt;
/* Vonalak darabsz�ma a CAN �zenet alapj�n, id�ben ARRAYSIZE darab egym�st k�vet�t t�rolunk el */
static lineType LineNumberArray[ARRAYSIZE];
//adott pillanatban h�ny darab vonalat �ll�tunk
lineType LineNumber = NoLine; //TODO lehetne static?
//el�z� pillanatban h�ny darab vonalat �ll�tottunk
static lineType LineNumberPrev = NoLine;

//milyen hosszan tart egy vonalt�pus
static float OneLineDistance=0;
static float TwoLineDistance=0;
static float ThreeLineDistance=0;
//vonalt�pusok kezd�pontja az abszol�t t�vols�gban
static float OneLineStartPos=0;
static float ThreeLineStartPos=0;


//TODO: t�rl�s!
//sz�molja mennyi ideig van egy darab vonal
uint32_t OneLineTime = 0;
//sz�molja mennyi ideig van h�rom darab vonal
uint32_t ThreeLineTime = 0;

/* �j �rt�k (FrontSensor_Data[1]) elment�se a t�mbbe (LineNumberArray[])
 * id�beli sz�r�s a vonalt�pusra: IsElementsEqual(LineNumberArray)
 *
 * sz�molja az id�t, am�g folyamatosan egy vagy h�rom vonal van: OneLineTime, ThreeLineTime
 * �ll�tja a State v�ltoz�t: SetSpeedState();
 */
void Do_GetLineType()
{
	//T_GETLINE ciklusid� (T_GETLINE*0.1ms) biztos�t�sa.
	if(TimeLineType > T_LINETYPE)
	{
		//sz�ml�l� null�z�s a ciklus �jrakezd�s�hez
		TimeLineType=0;

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

		//v�ltoz� �rt�k be�ll�t�s az �zenett�mbben(Debugszoftver)
		//TODO: m�shol k�ne megh�vni?
		//SetValue_AtMessageArray(var_LineNumber, (float)LineNumber);






		//t�rl�s
		if(LineNumber == OneLine)
		{
			//T_GETLINE*1ms egys�gekben
			OneLineTime++;
			ThreeLineTime=0;
		}
		if(LineNumber == ThreeLine)
		{
			//T_GETLINE*1ms egys�gekben
			ThreeLineTime++;
			//OneLineTime=0;
		}

		//sebess�g�llapot �ll�t�s
		SetSpeedState();

		//teszt
		if(LineNumber == ThreeLine)
			Led_On(Blue);
		else
			Led_Off(Blue);
	}
}

//figyeli, hogy r�g�ta egyvonal van-e, �s ha igen, Egyvonal_folyt-ba �ll�tja az �llapotot
void Is_EgyVonal()
{
	//ha most kezd�d�tt az egyvonalhossz, ezel�tt m�s volt, a kezdeti �rt�k
	if(LineNumberPrev != OneLine && LineNumber == OneLine)
	{
		OneLineStartPos = Encoder_GetDistance();
	}
	//egyvonal hossza jelenleg
	if(LineNumber == OneLine)
	{
		OneLineDistance = Encoder_GetDistance() - OneLineStartPos;
	}
	//hossz ellen�rz�s, ha el�g nagy, berakjuk egyvonal folytonosba
	if(OneLineDistance > ONELINEDISTANCE_FOLYT_LIMIT)
	{
		StateLineType = Egyvonal_folyt;
	}
}
//figyeli hogy h�romvonal van-e, �s ha igen, szaggatott vagy folytonos, �s be�ll�tja a StateLineType v�ltoz�t ennek megfelel�en
void Is_HaromVonal()
{
	//egy vonalb�l h�rom vonal lett
	if(LineNumber==ThreeLine && LineNumberPrev==OneLine)
	{
		//ha normal egyvonal �llapotban  voltunk
		if(StateLineType == Egyvonal_folyt)
		{
			//abszol�t t�vols�g kezd�pontja
			ThreeLineStartPos = Encoder_GetDistance();
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
		ThreeLineDistance = Encoder_GetDistance() - ThreeLineStartPos;
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
			ThreeLineDistance = Encoder_GetDistance() - ThreeLineStartPos;
			//az egyvonal t�vols�g�t az Is_EgyVonal() sz�molja folyamatosan
		}
	}
}

/* Be�ll�tja a sebess�g �llapotot az el�z� �llapot, �s a vonalt�pus alapj�n: egyenes vagy kanyar */
void SetSpeedState()
{
	//m�sodperces sz�ml�l�
	static uint8_t second=0;
	//TimeSpeedState:1ms, second:1000*1ms=1s
	if(TimeSpeedState > 1000)
	{
		TimeSpeedState=0;
		second++;
	}

	/* Ha eltelt SECONDLIMIT m�sodperc az el�z� �llopv�lt�s �ta,
	 * �s egy adott ideje(ContinousMinTime) h�rom darab vonal van,
	 * csak akkor v�lthat �jb�l �llapotot.
	 */
	if((second>SECONDLIMIT) )
	{
		/* egyenesb�l v�ltson kanyarra */
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
			/* kanyarb�l v�ltson egyenesbe */
			if((StateQ1==CornerOut || StateQ1==CornerIn) && (ThreeLineTime > ContinousMinTime_szagg))
			{
				StateQ1 = Straight;
				second=0;
				//�llapotv�lt�sn�l null�zzuk a vonalid�ket
				OneLineTime=0;
				ThreeLineTime=0;

				//teszt
				//Led_Off(Red);
			}
		}
	}

	//kanyarbej�ratb�l v�ltson kij�ratba
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

/* Id�beli sz�r�s a vonalt�pusra, akkor �ll�tjuk biztosra hogy h�ny darab vonal van,
 * ha t�bb egym�s ut�ni �rt�k is egyezik.
 * lineType t�pus� t�mb�t megvizsg�lja, egyenl�-e minden elem,
 * ha igen, 1-gyel t�r vissza, ha hamis, null�val*/
uint8_t IsElementsEqual(lineType* Array)
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
