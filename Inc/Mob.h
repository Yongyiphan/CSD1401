
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
	int HP;
	int DEF;
	int Speed;

	int Range;
	int Dmg;
	int size;
	
}MobStats;

typedef struct Mob {
	int Title;
	int SpawnCost;
	MobStats BaseStats;
	MobStats CStats;
	Coor coor;
} Mob;

typedef struct WaveTracker {
	int MobCount;
	int CurrentCount;
	int waveCost;
	int arrSize;
	Mob* arr;
}WaveTrack;




Mob CreateMob(int Title, MobStats Base, int xLeft, int xRight, int yTop, int yBtm, int offSet);
//void GenerateWaves(Mob *arr, int *MobQuantity, int waveCost, int *outMobCount);
//void GenerateWaves(WaveTrack *tracker);
void GenerateWaves(WaveTrack* tracker, int xLeft, int xRight, int yTop, int yBtm, int offSet);


void DrawMob(Mob *mob, int r, int g, int b);
void MobBasicAtk(Mob* mob, float tX, float tY);

MobStats CreateBaseStat(int type);
