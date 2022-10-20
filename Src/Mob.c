#include "Mob.h"
#include "cprocessing.h"
#include <stdlib.h>
#include <stdio.h>

int MobCosts[3] = {1,2,3};

Mob CreateMob(int Title, MobStats Base, int xLeft, int xRight, int yTop, int yBtm)
{
	Coor c = { (double)CP_Random_RangeFloat(xLeft, xRight), (double)CP_Random_RangeFloat(yTop, yBtm) };
	Mob m = {
		.Title = Title,
		.BaseStats = Base,
		.CStats = Base,
		.coor = c
	};
	//Allocate Size
	return m;
}

void AddMobToArr(Mob* container, Mob* m) {
	container = malloc(sizeof(Mob));
	container->Title = m->Title;
	container->BaseStats = m->BaseStats;
	container->CStats = m->CStats;
	container->coor = m->coor;
}


void DrawMob(Mob* mob)
{
	//Draw Circle
	CP_Settings_StrokeWeight(0.5f);
	int alpha = (mob->CStats.HP / mob->BaseStats.HP) * 255;
	CP_Settings_Fill(CP_Color_Create(255,255,255, alpha));
	CP_Graphics_DrawCircle((double) mob->coor.x, (double) mob->coor.y, mob->CStats.size);
}

//Generates Mobs
//void GenerateWaves(Mob *arr, int* arrSize, int waveCost, int *outMobCount) {
void GenerateWaves(WaveTrack *tracker) {
	int gMobCount = 0;
	int xLeft = 0, xRight = CP_System_GetWindowWidth(), yTop = 0, yBtm = CP_System_GetWindowHeight();
	int waveCost = tracker->waveCost;
	while (waveCost > 0) {
		int randMobI = CP_Random_RangeInt(0, 1);
		//int randMobI = 0;
		int randMobCost = MobCosts[randMobI];
	
		Mob m = CreateMob(randMobI, CreateBaseStat(randMobI), xLeft, xRight, yTop, yBtm);

		if (gMobCount-1 > tracker->arrSize) {
			int nQuantity = tracker->arrSize + 100;
			
			Mob *temp = realloc(tracker->arr, sizeof(Mob) * nQuantity);
			if (temp != NULL) {
				tracker->arr = temp;
				tracker->arrSize = nQuantity;
			}
			continue;
		}
	
		if (waveCost >= randMobCost) {
			tracker->arr[gMobCount] = m;
			Mob e = tracker->arr[gMobCount];
			//int  Titleat = e.Title;
			//int x = e.coor.x;
			//int y = e.coor.y;
			//printf("Pos: %d -> Title: %d | X: %d | Y: %d\n",gMobCount, Titleat,x, y);
			//printf("%p\n", &e);
			gMobCount += 1;
			waveCost -= randMobCost;
		}
	}
	tracker->MobCount = gMobCount;
	//for (int i = 0; i < gMobCount; i++) {
	//	Mob *t = &tracker->arr[i];
	//	int  Titleat = t->Title;
	//	int x = t->coor.x;
	//	int y = t->coor.y;
	//	printf("Pos: %d -> Title: %d | X: %d | Y: %d\n",i, Titleat,x, y);
	//	printf("%p\n", &t);
	//	
	//}

	//temp is free'ed
}

MobStats CreateBaseStat(int type) {
	switch (type) {
	case SmallMob:
		return (MobStats) {
			.HP = 5,
			.DEF = 0,
			.size = 10,
			.Speed = 0,
			.Dmg = 0,
			.Range = 0
		};
	case MediumMob:
		return (MobStats) {
			.HP = 5,
			.DEF = 0,
			.size = 20,
			.Speed = 0,
			.Dmg = 0,
			.Range = 0
		};
	case BigMob:
		return (MobStats) {
			.HP = 5,
			.DEF = 0,
			.size = 30,
			.Speed = 0,
			.Dmg = 0,
			.Range = 0
		};

	}
}

