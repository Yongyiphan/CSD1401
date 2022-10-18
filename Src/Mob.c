#include "Mob.h"
#include "CProcessing.h"
#include <stdlib.h>

Mob CreateMob(int Title, double hp, double def, double speed, int Range, double Dmg, double x, double y, int size)
{
	Mob m = {
		.Title = Title,
		.HP = hp,
		.DEF = def,
		.Speed = speed,
		.Range = Range,
		.Dmg = Dmg,
		.coor = (Coor) {x, y},
		.size = size
	};
	//Allocate Size

	return m;
}

void DrawMob(Mob* mob)
{
	//Draw Circle
	CP_Settings_Fill(CP_Color_Create(255,255,255,255));
	CP_Graphics_DrawCircle(mob->coor.x, mob->coor.y, mob->size);
}

void MobFactory(Mob arr[], int quantity, int mobCount) {
	int height = CP_System_GetWindowHeight(), width = CP_System_GetWindowWidth();
	int Title = 0, Range = 20, size = 25;
	double HP = 5, DEF = 5, Speed = 1, Dmg = 1;

	for (int i = 0; i < quantity; i++) {
		Coor coor = {drand(0,width), drand(0, height)};
		Mob m =  {.Title = Title, .HP = HP, .DEF = DEF, .Range = Range, .size = size, .Speed = Speed, .Dmg = Dmg, .coor = coor};
		arr[i] = m;
	}
}

double drand ( double low, double high )
{
    return ( (double)rand() * ( high - low ) ) / (double) RAND_MAX + low;
}
