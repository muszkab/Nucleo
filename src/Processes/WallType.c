/*
 * WallType.c
 *
 *  Created on: 2017. jan. 16.
 *      Author: Muszka
 */

#include "WallType.h"

//ciklusid�
#define T_WALLTYPE			100//ms

/********** Akad�lyfelismer�s **********/
//sharp-sharp: 23cm
//vonal-sharp: 10.5cm
#define GATEDISTANCE		25	//vonal-kapu: 31cm; milyen messze van maximum a kapu a sharp szenzort�l
#define WALLDISTANCE		18	//vonal-fal: 30cm; milyen messze van maximum a fal a sharp szenzort�l
#define BORDADISTANCE		18	//vonal-borda: 26cm; milyen messze van maximum a borda a sharp szenzort�l
#define KORFORGALOMDISTANCE	22	//vonal-k�rforgalom: 30cm; milyen messze van maximum a k�rforgalom a sharp szenzort�l
#define NOWALLDISTANCE		30	//ha enn�l nagyobb t�vols�got m�r�nk, akkor nincs fal
#define WALLLENGTH			40	//n�vleges: 60cm
#define SIMA_BORDA_SZORASHATAR	4	//sima: 0cm; bordas: 4cm a t�vols�g sz�r�sa

//�llapotv�ltoz�, mely mutatja milyen fal van glob�lisan az aut� mellett
static State_Wall StateWall_Global=Nincsfal;
//�llapotv�ltoz�, mely mutatja milyen fal van jobb oldalt az aut� mellett
static State_Wall Right_StateWall=Nincsfal;
//�llapotv�ltoz�, mely mutatja milyen fal van bal oldalt az aut� mellett
static State_Wall Left_StateWall=Nincsfal;
//jobb oldali sharp �ltal m�rt t�vols�g
static float RightSharpDistance=NOWALLDISTANCE+10;
//bal oldali sharp �ltal m�rt t�vols�g
static float LeftSharpDistance=NOWALLDISTANCE+10;
//milyen t�vol volt a jobboldali fal minimum
static float RightWall_Distance_Min=NOWALLDISTANCE+10;
//milyen t�vol volt a baloldali fal minimum
static float LeftWall_Distance_Min=NOWALLDISTANCE+10;
//milyen t�vol volt a jobboldali fal maximum
static float RightWall_Distance_Max=0;
//milyen t�vol volt a baloldali fal maximum
static float LeftWall_Distance_Max=0;
//milyen hosszan tart a jobboldali fal
static float RightWall_Length=0;
//milyen hosszan tart a baloldali fal
static float LeftWall_Length=0;
//hosszm�r�shez mindig elt�roljuk az el�z� pillanatbeli abszol�t megtett utat, ezt vonjuk ki a jelenlegi abszol�t �tb�l
static float RightWall_Length_AbsolutePos=0;
//hosszm�r�shez mindig elt�roljuk az el�z� pillanatbeli abszol�t megtett utat, ezt vonjuk ki a jelenlegi abszol�t �tb�l
static float LeftWall_Length_AbsolutePos=0;

/* Static f�ggv�nyek */
static void Is_Wall_EachSide(const float *SharpDistance, State_Wall *StateWall, float *WallDistance_Min, float *WallDistance_Max,
		float* Wall_Length_AbsolutePos, float* Wall_Length);
static void Is_Wall_Global();
static void Is_Korforgalom();


/*
 * Figyeli hogy vannak-e falak, milyen t�pus�ak, �s aszerint �ll�tja a StateWall_Global �llapotv�ltoz�t
 */
void Do_WallType()
{
	if(TimeWallType > T_WALLTYPE)
	{
		//t�vols�g �rt�kek lek�r�se
		RightSharpDistance = GetRightSharpDistance();
		LeftSharpDistance = GetLeftSharpDistance();

		//jobb oldali fal figyel�se
		Is_Wall_EachSide(&RightSharpDistance, &Right_StateWall, &RightWall_Distance_Min, &RightWall_Distance_Max,
				&RightWall_Length_AbsolutePos, &RightWall_Length);
		//bal oldali fal figyel�se
		Is_Wall_EachSide(&LeftSharpDistance, &Left_StateWall, &LeftWall_Distance_Min, &LeftWall_Distance_Max,
				&LeftWall_Length_AbsolutePos, &LeftWall_Length);

		//teljes �llapot �ssze�ll�t�sa a k�t oldal �llapota alapj�n
		Is_Wall_Global();

		//TODO: lehet h nem itt kell figyelni
		//ha a szaggatott vonal jelzi a k�rforgalmat, figyelj�k melyik oldalt van
		if(Get_StateLineType() == Egyvonal_szagg)
		{
			Is_Korforgalom();
		}
	}
}

//Right_StateWall �s Left_StateWall �llapota alapj�n be�ll�tja a glob�lis StateWall v�ltoz�t
static void Is_Wall_Global()
{
	//jobb oldali fal �llapotai alapj�n megy�nk v�gig az eseteken, bel�l vizsg�ljuk a bal oldali falat
	switch(Right_StateWall){
		case(Nincsfal):
				if(Left_StateWall == Nincsfal)
				{
					if(StateWall_Global != Nincsfal)
					{
						StateWall_Global = Nincsfal;
						SendDebugMessage_Text("Nincsfal");
					}
				}
				//ha kapu �llapotban az egyik falnak hamarabb v�ge lenne, csak egy falat �rz�kelne, �s rossz �llapotba ker�lne
				else if(Left_StateWall == Simafal && StateWall_Global != Kapu)
				{
					if(StateWall_Global != Simafal_bal)
					{
						StateWall_Global = Simafal_bal;
						SendDebugMessage_Text("Simafal_bal");
					}
				}
				else if(Left_StateWall == Bordasfal)
				{
					if(StateWall_Global != Bordasfal_bal)
					{
						StateWall_Global = Bordasfal_bal;
						SendDebugMessage_Text("Bordasfal_bal");
					}
				}
			break;

		case(Simafal):
				if(Left_StateWall == Nincsfal && StateWall_Global != Kapu)
				{
					if(StateWall_Global != Simafal_jobb)
					{
						StateWall_Global = Simafal_jobb;
						SendDebugMessage_Text("Simafal_jobb");
					}
				}
				else if(Left_StateWall == Simafal)
				{
					if(StateWall_Global != Kapu)
					{
						StateWall_Global = Kapu;
						SendDebugMessage_Text("Kapu");
					}
				}
				else if(Left_StateWall == Bordasfal)
				{
					if(StateWall_Global != Hiba)
					{
						StateWall_Global = Hiba;
						SendDebugMessage_Text("Hiba a falnal!");
					}
				}
			break;

		case(Bordasfal):
				if(Left_StateWall == Nincsfal)
				{
					if(StateWall_Global != Bordasfal_jobb)
					{
						StateWall_Global = Bordasfal_jobb;
						SendDebugMessage_Text("Bordasfal_jobb");
					}
				}
				else //if(Left_StateWall != Fal) //csak egyszer� fal �llapotban lehet, a t�bbi eset hib�s �llapot
				{
					if(StateWall_Global != Hiba)
					{
						StateWall_Global = Hiba;
						SendDebugMessage_Text("Hiba a falnal!");
					}
				}
			break;
		default:

			break;

	}//switch-case
}//f�gv�ny

//figyeli milyen fal a sharp szenzor mellett. meg lehet h�vni a jobb �s baloldali szenzorra is
static void Is_Wall_EachSide(const float *SharpDistance, State_Wall *StateWall, float *WallDistance_Min, float *WallDistance_Max,
		float* Wall_Length_AbsolutePos, float* Wall_Length)
{
	//ha nem jelez akad�lyt, az �llapotot nincsfalra �ll�tjuk
	if(*SharpDistance > WALLDISTANCE)
	{
		*StateWall = Nincsfal;
	}
	//ha a szenzor akad�lyt jelez
	else if(*SharpDistance < WALLDISTANCE)
	{
		//ha most el�sz�r jelez akad�lyt
		if(*StateWall == Nincsfal)
		{
			*WallDistance_Min = *SharpDistance;
			*WallDistance_Max = *SharpDistance;
			*Wall_Length_AbsolutePos = Encoder_GetDistance_cm();
			*StateWall = Fal;
		}
		//ha m�r fal �llapot volt
		else if(*StateWall == Fal)
		{
			//minimum t�vols�g
			if(*SharpDistance < *WallDistance_Min)
				*WallDistance_Min = *SharpDistance;
			//maximum t�vols�g
			if(*SharpDistance > *WallDistance_Max)
				*WallDistance_Max = *SharpDistance;

			//fal hossza: hozz�adjuk az egy cikkus alatt megtett kis �tszakaszt, majd lementj�k az el�z� abszol�t utat
			*Wall_Length += Encoder_GetDistance_cm() - *Wall_Length_AbsolutePos;
			*Wall_Length_AbsolutePos = Encoder_GetDistance_cm();

			//leellen�rizz�k a fal hossz�t, hogy el�g hossz�-e m�r
			if(*Wall_Length > WALLLENGTH)
			{
				//megn�zz�k mennyivel t�r el a min �s max t�vols�g �rt�k, ha kicsivel sima fal, ha sokkal bord�s fal
				if((*WallDistance_Max - *WallDistance_Min) < SIMA_BORDA_SZORASHATAR)
				{
					*StateWall = Simafal;
				}
				else
				{
					*StateWall = Bordasfal;
				}
			}
		}
	}
}

static void Is_Korforgalom()
{
	//TODO: lehet hogy hamarabb visszav�lt norm�l egyvonal folytonosba!
	if(Get_StateLineType() == Egyvonal_szagg && StateWall_Global == Nincsfal)
	{
		//ha a jobb oldali szenzor akad�lyt jelez
		if(RightSharpDistance < KORFORGALOMDISTANCE)
		{
			StateWall_Global = Korforgalom_jobb;
		}
		//ha a bal oldali szenzor akad�lyt jelez
		if(LeftSharpDistance < KORFORGALOMDISTANCE)
		{
			StateWall_Global = Korforgalom_bal;
		}
	}
}

/* Get f�ggv�nyek */
State_Wall Get_StateWall()
{
	return StateWall_Global;
}

/* Set f�ggv�nyek */
void Set_StateWall(State_Wall newStateWall)
{
	StateWall_Global = newStateWall;
}
