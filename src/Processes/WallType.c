/*
 * WallType.c
 *
 *  Created on: 2017. jan. 16.
 *      Author: Muszka
 */

#include "WallType.h"

//ciklusidõ
#define T_WALLTYPE			100//ms

/********** Akadályfelismerés **********/
//sharp-sharp: 23cm
//vonal-sharp: 10.5cm
#define GATEDISTANCE		25	//vonal-kapu: 31cm; milyen messze van maximum a kapu a sharp szenzortól
#define WALLDISTANCE		18	//vonal-fal: 30cm; milyen messze van maximum a fal a sharp szenzortól
#define BORDADISTANCE		18	//vonal-borda: 26cm; milyen messze van maximum a borda a sharp szenzortól
#define KORFORGALOMDISTANCE	22	//vonal-körforgalom: 30cm; milyen messze van maximum a körforgalom a sharp szenzortól
#define NOWALLDISTANCE		30	//ha ennél nagyobb távolságot mérünk, akkor nincs fal
#define WALLLENGTH			40	//névleges: 60cm
#define SIMA_BORDA_SZORASHATAR	4	//sima: 0cm; bordas: 4cm a távolság szórása

//állapotváltozó, mely mutatja milyen fal van globálisan az autó mellett
static State_Wall StateWall_Global=Nincsfal;
//állapotváltozó, mely mutatja milyen fal van jobb oldalt az autó mellett
static State_Wall Right_StateWall=Nincsfal;
//állapotváltozó, mely mutatja milyen fal van bal oldalt az autó mellett
static State_Wall Left_StateWall=Nincsfal;
//jobb oldali sharp által mért távolság
static float RightSharpDistance=NOWALLDISTANCE+10;
//bal oldali sharp által mért távolság
static float LeftSharpDistance=NOWALLDISTANCE+10;
//milyen távol volt a jobboldali fal minimum
static float RightWall_Distance_Min=NOWALLDISTANCE+10;
//milyen távol volt a baloldali fal minimum
static float LeftWall_Distance_Min=NOWALLDISTANCE+10;
//milyen távol volt a jobboldali fal maximum
static float RightWall_Distance_Max=0;
//milyen távol volt a baloldali fal maximum
static float LeftWall_Distance_Max=0;
//milyen hosszan tart a jobboldali fal
static float RightWall_Length=0;
//milyen hosszan tart a baloldali fal
static float LeftWall_Length=0;
//hosszméréshez mindig eltároljuk az elõzõ pillanatbeli abszolút megtett utat, ezt vonjuk ki a jelenlegi abszolút útból
static float RightWall_Length_AbsolutePos=0;
//hosszméréshez mindig eltároljuk az elõzõ pillanatbeli abszolút megtett utat, ezt vonjuk ki a jelenlegi abszolút útból
static float LeftWall_Length_AbsolutePos=0;

/* Static függvények */
static void Is_Wall_EachSide(const float *SharpDistance, State_Wall *StateWall, float *WallDistance_Min, float *WallDistance_Max,
		float* Wall_Length_AbsolutePos, float* Wall_Length);
static void Is_Wall_Global();
static void Is_Korforgalom();


/*
 * Figyeli hogy vannak-e falak, milyen típusúak, és aszerint állítja a StateWall_Global állapotváltozót
 */
void Do_WallType()
{
	if(TimeWallType > T_WALLTYPE)
	{
		//távolság értékek lekérése
		RightSharpDistance = GetRightSharpDistance();
		LeftSharpDistance = GetLeftSharpDistance();

		//jobb oldali fal figyelése
		Is_Wall_EachSide(&RightSharpDistance, &Right_StateWall, &RightWall_Distance_Min, &RightWall_Distance_Max,
				&RightWall_Length_AbsolutePos, &RightWall_Length);
		//bal oldali fal figyelése
		Is_Wall_EachSide(&LeftSharpDistance, &Left_StateWall, &LeftWall_Distance_Min, &LeftWall_Distance_Max,
				&LeftWall_Length_AbsolutePos, &LeftWall_Length);

		//teljes állapot összeállítása a két oldal állapota alapján
		Is_Wall_Global();

		//TODO: lehet h nem itt kell figyelni
		//ha a szaggatott vonal jelzi a körforgalmat, figyeljük melyik oldalt van
		if(Get_StateLineType() == Egyvonal_szagg)
		{
			Is_Korforgalom();
		}
	}
}

//Right_StateWall és Left_StateWall állapota alapján beállítja a globális StateWall változót
static void Is_Wall_Global()
{
	//jobb oldali fal állapotai alapján megyünk végig az eseteken, belül vizsgáljuk a bal oldali falat
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
				//ha kapu állapotban az egyik falnak hamarabb vége lenne, csak egy falat érzékelne, és rossz állapotba kerülne
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
				else //if(Left_StateWall != Fal) //csak egyszerû fal állapotban lehet, a többi eset hibás állapot
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
}//fügvény

//figyeli milyen fal a sharp szenzor mellett. meg lehet hívni a jobb és baloldali szenzorra is
static void Is_Wall_EachSide(const float *SharpDistance, State_Wall *StateWall, float *WallDistance_Min, float *WallDistance_Max,
		float* Wall_Length_AbsolutePos, float* Wall_Length)
{
	//ha nem jelez akadályt, az állapotot nincsfalra állítjuk
	if(*SharpDistance > WALLDISTANCE)
	{
		*StateWall = Nincsfal;
	}
	//ha a szenzor akadályt jelez
	else if(*SharpDistance < WALLDISTANCE)
	{
		//ha most elõször jelez akadályt
		if(*StateWall == Nincsfal)
		{
			*WallDistance_Min = *SharpDistance;
			*WallDistance_Max = *SharpDistance;
			*Wall_Length_AbsolutePos = Encoder_GetDistance_cm();
			*StateWall = Fal;
		}
		//ha már fal állapot volt
		else if(*StateWall == Fal)
		{
			//minimum távolság
			if(*SharpDistance < *WallDistance_Min)
				*WallDistance_Min = *SharpDistance;
			//maximum távolság
			if(*SharpDistance > *WallDistance_Max)
				*WallDistance_Max = *SharpDistance;

			//fal hossza: hozzáadjuk az egy cikkus alatt megtett kis útszakaszt, majd lementjük az elõzõ abszolút utat
			*Wall_Length += Encoder_GetDistance_cm() - *Wall_Length_AbsolutePos;
			*Wall_Length_AbsolutePos = Encoder_GetDistance_cm();

			//leellenõrizzük a fal hosszát, hogy elég hosszú-e már
			if(*Wall_Length > WALLLENGTH)
			{
				//megnézzük mennyivel tér el a min és max távolság érték, ha kicsivel sima fal, ha sokkal bordás fal
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
	//TODO: lehet hogy hamarabb visszavált normál egyvonal folytonosba!
	if(Get_StateLineType() == Egyvonal_szagg && StateWall_Global == Nincsfal)
	{
		//ha a jobb oldali szenzor akadályt jelez
		if(RightSharpDistance < KORFORGALOMDISTANCE)
		{
			StateWall_Global = Korforgalom_jobb;
		}
		//ha a bal oldali szenzor akadályt jelez
		if(LeftSharpDistance < KORFORGALOMDISTANCE)
		{
			StateWall_Global = Korforgalom_bal;
		}
	}
}

/* Get függvények */
State_Wall Get_StateWall()
{
	return StateWall_Global;
}

/* Set függvények */
void Set_StateWall(State_Wall newStateWall)
{
	StateWall_Global = newStateWall;
}
