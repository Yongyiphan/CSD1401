

#define SmallMob 0
#define MediumMob 1
#define BigMob 2
#define RangedMob 3
#define BigBoss 4

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


///*
//Mob C Flow
//: Generate N amt of mobs at random locations
//: Allow Random Movement of Mobs Towards Player
//
//Mob Cycle:
//: Init -> Generate mobs as per waves count; (Implemented)
//: Every "Min" / All Mob on screens' death -> Generate New waves
//------------
//: 5 Wave Pool of memory of size Mob * MobPoolQuantity 
//: Spawn  Wave tracks amt of mobs (Waves Mob <= MobPoolQuantity) else realloc more memory
//: Decrement wave track when mobs die
//	: IF all mobs in waves die. Spawn new waves. 
//	: Mob's transparency drops (scaled with HP)
//
//Print Conditions
//: Waves with mob count > 0
//
//*/
//


//Mob CreateMob(int Title, MobStats Base, int xLeft, int xRight, int yTop, int yBtm, int offSet);
Mob CreateMob(int Title, MobStats Base, int playerX, int playerY, int offSet);
//void GenerateWaves(Mob *arr, int *MobQuantity, int waveCost, int *outMobCount);
//void GenerateWaves(WaveTrack *tracker);
//void GenerateWaves(WaveTrack* tracker, int xLeft, int xRight, int yTop, int yBtm, int offSet);
void GenerateWaves(WaveTrack* tracker, int playerX, int playerY, int offSet);


void DrawMob(Mob *mob, int r, int g, int b);
void MobPathFinding(Mob* mob, float tX, float tY);

MobStats CreateBaseStat(int type);
