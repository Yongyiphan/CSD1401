#include "cprocessing.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Mob.h"
#include "player.h"

//Pool of Mob Types
//Edit here for different Mob Types
int MobCosts[3] = {1,2,3};
MobStats CreateBaseStat(int type) {
	switch (type) {
		//Mob Format:
		//int HP;
		//int DEF;
		//int Speed;

		//int Range;
		//int Dmg;
		//int size;
		case SmallMob:
			return (MobStats) {
				.HP = 5, .DEF = 10, .Speed = 5, .Range = 0, .Dmg = 1, .size = 10
			};
		case MediumMob:
			return (MobStats) {
				.HP = 5, .DEF = 10, .Speed = 5, .Range = 0, .Dmg = 1, .size = 15
			};
		case BigMob:
			return (MobStats) {
				.HP = 5, .DEF = 10, .Speed = 5, .Range = 0, .Dmg = 1, .size = 25
			};
		case RangedMob:
			return (MobStats) {
				.HP = 5, .DEF = 10, .Speed = 5, .Range = 0, .Dmg = 1, .size = 25
			};
		case BigBoss:
			return (MobStats) {
				.HP = 5, .DEF = 10, .Speed = 5, .Range = 0, .Dmg = 1, .size = 50
			};
		default:
			//Something Might be wrong;
			return (MobStats) {
				.HP = 5, .DEF = 10, .Speed = 5, .Range = 0, .Dmg = 1, .size = 100
			};

	}
}
Mob CreateMob(int Title, MobStats Base, int xLeft, int xRight, int yTop, int yBtm, int offSet)
{
	//Assume Player center of spawnable area
	//Player coor = xRight - xLeft, yBtm - yTop
	Coor c, PlayerCoor = { (xRight - xLeft) / 2, (yBtm - yTop) / 2 };
	int Diff = 0;
	do {
		c = (Coor) {(double)CP_Random_RangeFloat(xLeft, xRight), (double)CP_Random_RangeFloat(yTop, yBtm)};
		Diff = sqrt(pow(c.x - PlayerCoor.x, 2) + pow(c.y - PlayerCoor.y, 2));
	} while (Diff <= offSet);

	Mob m = {
		.Title = Title,
		.BaseStats = Base,
		.CStats = Base,
		.coor = c
	};
	//Allocate Size
	return m;
}

//Function that generated Mobs based off a cost system
// Input parameters will be a wavetrack struct (refer to Mob.h)
// xLeft, xRight, yTop, yBtm denotes the areas of which mobs can be generated in
//OffSet Prevents mobs from being spawned in area around Player
void GenerateWaves(WaveTrack *tracker, int xLeft, int xRight, int yTop, int yBtm, int offSet) {
	//gMobCount = Generated Mob Count throughout this func
	//waveCost = Amt of "currency" the func will take to generated random types of mobs per wave
	int gMobCount = 0, waveCost = tracker->waveCost;
	//int xLeft = 0, xRight = CP_System_GetWindowWidth(), yTop = 0, yBtm = CP_System_GetWindowHeight();
	while (waveCost > 0) {
		//Generate a random mob from mob types pool
		//Edit here to generated different Mob Types
		int randMobI = CP_Random_RangeInt(0, 2);
		int randMobCost = MobCosts[randMobI];
	
		//Algo to extend array size when nearing max capacity
		//Provides array with overhead for additional 100 Mobs.
		if (gMobCount >= tracker->arrSize) {
			int nQuantity = tracker->arrSize + 100;
			
			Mob *temp = realloc(tracker->arr, sizeof(Mob) * nQuantity);
			if (temp != NULL) {
				//IF realloc is successful
				//Update tracker with new parameters
				tracker->arr = temp;
				tracker->arrSize = nQuantity;
			}
			printf("\n\tArray Expended\n\n");
			printf("\n\tNew Array Size: %d\n", tracker->arrSize);
			continue;
		}
		
		//Algo to add Mob generated to array.
		if (waveCost >= randMobCost) {
			//Generated a new mob at specified locations
			Mob m = CreateMob(randMobI, CreateBaseStat(randMobI), xLeft, xRight, yTop, yBtm, offSet);
			tracker->arr[gMobCount] = m;
			printf("Pos: %d -> Title: %d | X: %d | Y: %d\n",gMobCount,m.Title,(int) m.coor.x,(int) m.coor.y);
			//printf("%p\n", &m);
			
			
			gMobCount += 1;
			waveCost -= randMobCost;
		}
	}
	tracker->MobCount = gMobCount;
	tracker->CurrentCount = gMobCount;
}

void MobPathFinding(Mob* mob, float tX, float tY) {
	CP_Vector v;
	switch (mob->Title) {
	default:
		v = CP_Vector_Normalize(CP_Vector_Set(mob->coor.x - tX, mob->coor.y - tY));
		mob->coor.x -= v.x * mob->CStats.Speed;
		mob->coor.y -= v.y * mob->CStats.Speed;
		break;
	}
}

void DrawMob(Mob* mob, int r, int g, int b)
{
	//Draw Circle
	CP_Settings_StrokeWeight(0.5f);
	int alpha = (mob->CStats.HP / mob->BaseStats.HP) * 255;
	CP_Settings_Fill(CP_Color_Create(r,g,b, alpha));
	CP_Graphics_DrawCircle((double) mob->coor.x, (double) mob->coor.y, mob->CStats.size);
}


