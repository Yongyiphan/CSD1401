#include "Mob.h"
#include "CProcessing.h"
#include <stdlib.h>

int MobCosts[3] = {1,2,3};

Mob CreateMob(int Title, MobStats Base, Coor coor)
{
	Mob m = {
		.Title = Title,
		.BaseStats = Base,
		.CStats = Base,
		.coor = coor
	};
	//Allocate Size

	return m;
}

void DrawMob(Mob* mob)
{
	//Draw Circle
	int alpha = (mob->CStats.HP / mob->BaseStats.HP) * 255;
	CP_Settings_Fill(CP_Color_Create(255,255,255, alpha));
	CP_Graphics_DrawCircle(mob->coor.x, mob->coor.y, mob->CStats.size);
}

//Generates Mobs
void GenerateWaves(Mob *arr, int *quantity, int waveCost) {
	//quantity = upper limit of mobs that can be created
	int height = CP_System_GetWindowHeight(), width = CP_System_GetWindowWidth();
	int generatedMobCount = 0, newQuantity;
	Mob currentMob;
	while (waveCost > 0) {
		int randMob = CP_Random_RangeInt(0, 2);
		int randMobCost = MobCosts[randMob];
	
		//Overhead of 100;
		if (generatedMobCount > *quantity) {
			newQuantity = *quantity + 100;
			Mob *temp = realloc(arr, sizeof(Mob) * newQuantity);
			if (temp != NULL) {
				arr = temp;
				*quantity = newQuantity;
			}
		}

		if (waveCost >= randMobCost) {
			Coor coor = {(double) CP_Random_RangeFloat(0,width), (double) CP_Random_RangeFloat(0, height)};
			arr[generatedMobCount] = CreateMob(randMob, CreateBaseStat(randMob), coor);
			waveCost -= randMobCost;
			generatedMobCount+=1;
		}
	}

//	for (int i = 0; i < quantity; i++) {
//		Coor coor = {(double) CP_Random_RangeFloat(0,width), (double) CP_Random_RangeFloat(0, height)};
//		Mob m = CreateMob(Title,, coor);
//		arr[i] = m;
//	}
}

void GenerateMobs() {

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

