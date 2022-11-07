#pragma once
#ifndef MOB_H
#define MOB_H

#include "player.h"
#include "cprocessing.h"

#define MobTypes 5
#define SmallMob 0
#define MediumMob 1
#define BigMob 2
#define RangedMob 3
#define BigBoss 4



typedef struct MobBase {
	float HP;
	float DEF;
	float Speed;

	int Range;
	int Dmg;
	int size;

}MobStats;

typedef struct Mob {
	int Title;

	MobStats BaseStats;
	MobStats CStats;
	float x, y;
	int Status; 
	int AnimationCycle;
	int w, h;
	//Dead = 0 | Alive = 1
} Mob;

 

typedef struct WaveTracker {
	int MaxMob;
	int MobCount;
	int CurrentCount;
	int WaveCost;
	int arrSize;
	int spawnOffset;
	Mob** arr;
}WaveTrack;

//Mob Image
#define Img_Scale 80
#define BoundScale 1.2f

extern CP_Image** MobSprites;
extern int Mob_Img;
// --------------------------- //

//Mob Stuff
#define NO_WAVES 1
#define Spawn_Timer 5
#define Wave_Timer 20
#define MaxMobGrowthRate 5
#define MaxUpperLimit 20
#define WaveCostGrowthRate 1
#define SpawnAreaOffset 500
#define StartMobQuantity 150

extern int WaveIDQueue[NO_WAVES], MobCount[NO_WAVES], CWave, CWaveCost, CMaxMob;
extern WaveTrack WaveTracker[NO_WAVES];
/*
Mob C Flow
: Generate N amt of mobs at random locations
: Allow Random Movement of Mobs Towards Player

Mob Cycle:
: Init -> Generate mobs as per waves count; (Implemented)
: Every "Min" / All Mob on screens' death -> Generate New waves
------------
: 5 Wave Pool of memory of size Mob * MobPoolQuantity 
: Spawn  Wave tracks amt of mobs (Waves Mob <= MobPoolQuantity) else realloc more memory
: Decrement wave track when mobs die
	: IF all mobs in waves die. Spawn new waves. 
	: Mob's transparency drops (scaled with HP)

Print Conditions
: Waves with mob count > 0

*/





void MobLoadImage(void);
void CreateWaveTracker(void);
void InitWavesArr(WaveTrack* tracker, int start);

void CreateBaseStat(MobStats* ms, int type);
void CreateMob(Mob* m, MobStats *Base, Player*player, int offSet);
void GenerateMobs(WaveTrack* tracker, Player* player);
//void GenerateWaves(Player* P, WaveTrack* queue, int* queueID, int WavesNo, int CostGrowth, int MaxMobGrowth,int *TotalWaveCount,  int* MobCount);
void GenerateWaves(Player* P);

void DrawMobImage(Mob* m, Player*p);

void MobTPlayerCollision(Mob* m, Player* p);
void MobTMobCollision(Mob* mob, Player* p, WaveTrack* tracker, int const No_Waves);

void PrintWaveStats(int*CWaveCount);

void FreeMobResource();

float squareDist(float one, float two);













#endif
