
/*
	Title
		: 0 => Mob
		: 1 => MiniBoss
		: 2 => Boss
*/

#define SmallMob 0
#define MediumMob 1
#define BigMob 2
#define BigBoss 3

typedef struct Coordinates {
	double x;
	double y;
}Coor;

typedef struct MobBase {
	double HP;
	double DEF;
	double Speed;

	int Range;
	double Dmg;
	int size;
	
}MobStats;

typedef struct Mob {
	int Title;
	int SpawnCost;
	MobStats BaseStats;
	MobStats CStats;
	Coor coor;
} Mob;




Mob CreateMob(int Title, MobStats Base, Coor coor);

void GenerateWaves(Mob arr[], int *MobQuantity, int waveCost);


void DrawMob(Mob *mob);

MobStats CreateBaseStat(int type);
