#include "cprocessing.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "player.h"
#include "Mob.h"

//#include "player.h"

//Pool of Mob Types
//Edit here for different Mob Types
int MobCosts[3] = {1,2,3};
void CreateBaseStat(MobStats* cStat, int type) {
	switch (type) {
		//Mob Format:
		//int HP;
		//int DEF;
		//int Speed;

		//int Range;
		//int Dmg;
		//int size;
		case SmallMob:
			cStat->HP = 5;
			cStat->DEF = 10;
			cStat->Speed = 5;
			cStat->Range = 0;
			cStat->Dmg = 1;
			cStat->size = 10;
			break;
		case MediumMob:
			cStat->HP = 5;
			cStat->DEF = 10;
			cStat->Speed = 5;
			cStat->Range = 0;
			cStat->Dmg = 1;
			cStat->size = 15;
			break;
		case BigMob:
			cStat->HP = 5;
			cStat->DEF = 10;
			cStat->Speed = 5;
			cStat->Range = 0;
			cStat->Dmg = 1;
			cStat->size = 25;
			break;
		case RangedMob:
			cStat->HP = 5;
			cStat->DEF = 10;
			cStat->Speed = 5;
			cStat->Range = 0;
			cStat->Dmg = 1;
			cStat->size = 10;
			break;
		case BigBoss:
			cStat->HP = 5;
			cStat->DEF = 10;
			cStat->Speed = 5;
			cStat->Range = 0;
			cStat->Dmg = 1;
			cStat->size = 50;
			break;
		default:
			//Something Might be wrong;
			cStat->HP = 5;
			cStat->DEF = 10;
			cStat->Speed = 5;
			cStat->Range = 0;
			cStat->Dmg = 1;
			cStat->size = 200;
			break;
		
	}
}
//Mob CreateMob(int Title, MobStats Base,int xLeft, int xRight, int yTop,int yBtm , int offSet)
void CreateMob(Mob*m, int Title, MobStats *Base, Player*player, int offSet)
{
	int SWidth = CP_System_GetWindowWidth(), SHeight = CP_System_GetWindowHeight();
	CP_Vector centerOffset = CP_Vector_Set(SWidth / 2.0f, SHeight / 2.0f);
	//Assume Player center of spawnable area
	//Player coor = xRight - xLeft, yBtm - yTop
	Coor c;
	float xLeft = player->x - centerOffset.x , xRight = player->x + centerOffset.x, yTop = player->y - centerOffset.y, yBtm = player->y + centerOffset.y, Diff = 0.0;
	do {
		c = (Coor) {(double)CP_Random_RangeFloat(xLeft, xRight), (double)CP_Random_RangeFloat(yTop, yBtm)};
		Diff = CP_Math_Distance(c.x, c.y, player->x, player->y);
	} while (Diff <= offSet);

	m->Title = Title;
	m->BaseStats = *Base;
	m->CStats = *Base;
	m->coor = c;
	m->Status = 1;
}


//Function that generated Mobs based off a cost system
// Input parameters will be a wavetrack struct (refer to Mob.h)
// xLeft, xRight, yTop, yBtm denotes the areas of which mobs can be generated in
//OffSet Prevents mobs from being spawned in area around Player
//void GenerateWaves(WaveTrack *tracker, int xLeft, int xRight, int yTop, int yBtm, int offSet) {
void GenerateWaves(WaveTrack *tracker, Player*player) {
	//gMobCount = Generated Mob Count throughout this func
	//waveCost = Amt of "currency" the func will take to generated random types of mobs per wave
	int gMobCount = 0, waveCost = tracker->waveCost;
	int Reused = 0;

	//int xLeft = 0, xRight = CP_System_GetWindowWidth(), yTop = 0, yBtm = CP_System_GetWindowHeight();
	while (waveCost > 0) {
		//Generate a random mob from mob types pool
		//Edit here to generated different Mob Types
		int randMobI = CP_Random_RangeInt(0, 2);
		int randMobCost = MobCosts[randMobI];
	
		//Algo to extend array size when nearing max capacity
		//Provides array with overhead for additional 100 Mobs.
		if (gMobCount >= tracker->arrSize) {
			int nQuantity = tracker->arrSize + 1000;
			
			Mob *temp = realloc(tracker->arr, sizeof(Mob) * nQuantity);
			if (temp != NULL) {
				//IF realloc is successful
				//Update tracker with new parameters
				tracker->arr = temp;
				tracker->arrSize = nQuantity;
			}
			printf("\n\tArray Expended");
			printf("\n\tNew Array Size: %d\n", tracker->arrSize);
			continue;
		}
		
		//Algo to add Mob generated to array.
		if (waveCost >= randMobCost) {
			if (tracker->MobCount > 0 && tracker->arr[gMobCount].Status == 0) {
				CreateBaseStat(&tracker->arr[gMobCount].BaseStats, randMobI);
				CreateMob(&tracker->arr[gMobCount], randMobI, &tracker->arr[gMobCount].BaseStats, player, tracker->spawnOffset);
				Reused += 1;

			}
			else {
				Mob m;
				MobStats ms;
				CreateBaseStat(&ms, randMobI);
				CreateMob(&m, randMobI, &ms, player, tracker->spawnOffset);
				tracker->arr[gMobCount] = m;
			}
			//Generated a new mob at specified locations
			//printf("Pos: %d -> Title: %d | X: %d | Y: %d\n",gMobCount,m.Title,(int) m.coor.x,(int) m.coor.y);
			//printf("%p\n", &m);
			
			gMobCount += 1;
			waveCost -= randMobCost;
		}
	}
	tracker->MobCount = gMobCount;
	tracker->CurrentCount = gMobCount;
	printf("Mobs Resused: %d\n", Reused);
	printf("New Mobs Created: %d\n", gMobCount - Reused);
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

void MobCollision(Mob* mob, Player *player) {
	//Drop items?
	//Drop HP?
	if (CP_Math_Distance(mob->coor.x, mob->coor.y, player->x, player->y) <= mob->CStats.size + player->HITBOX) {
		mob->CStats.HP -= player->DAMAGE;
		player->CURRENT_HP -= mob->CStats.Dmg;
	}
	if (mob->CStats.HP <= 0) {
		mob->Status = 0;
	}
}

