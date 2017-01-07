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

//vonaldarabsz�m sz�r�shez haszn�lt t�mb m�rete
#define ARRAYSIZE	3
//vonaldarabsz�m sz�r�sn�l, mennyi �rt�k lehet k�l�nb�z� a t�mbben, amikor m�g egy�rtelm�nek mondjuk a t�mb�t
#define TOLERANCE 	2	//nem haszn�ljuk jelenleg

/* V�ltoz�k */
/* Vonalak darabsz�ma a CAN �zenet alapj�n, id�ben ARRAYSIZE darab egym�st k�vet�t t�rolunk el */
lineType LineNumberArray[ARRAYSIZE];
//adott pillanatban h�ny darab vonalat �ll�tunk
lineType LineNumber = NoLine;
//el�z� pillanatban h�ny darab vonalat �ll�tottunk
lineType LineNumberPrev = NoLine;

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

		//egy vonalb�l h�rom vonal lett
		if(LineNumber==ThreeLine && LineNumberPrev==OneLine)
		{

		}

		//h�rom vonalb�l egy vonal lett
		if(LineNumber==OneLine && LineNumberPrev==ThreeLine)
		{

		}

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

		//v�ltoz� �rt�k be�ll�t�s az �zenett�mbben(Debugszoftver)
		//TODO: m�shol k�ne megh�vni?
		//SetValue_AtMessageArray(var_LineNumber, (float)LineNumber);
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
