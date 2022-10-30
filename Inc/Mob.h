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

typedef struct Coordinates {
	float x;
	float y;
}Coor;

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
	float x;
	float y;
	int Status; 

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
	CP_Color waveColor;
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
void MobLoadImage(CP_Image *Sprites, int arrSize);
void InitWavesArr(WaveTrack* tracker);
void CreateBaseStat(MobStats* ms, int type);
void CreateMob(Mob* m, MobStats *Base, Player*player, int offSet);
void GenerateMobs(WaveTrack* tracker, Player* player);
void GenerateWaves(Player* P, WaveTrack* queue, int* queueID, int WavesNo, int CostGrowth, int MaxMobGrowth,int *TotalWaveCount,  int* MobCount);

void DrawMobImage(CP_Image* Sprites, Mob* m);
void DrawMob(Mob *mob, int r, int g, int b);
void MobPathFinding(Mob* mob, float tX, float tY);
void MobCollision(Mob* mob, Player* player);

void PrintWaveStats(int*CWaveCount, int No_Waves, int*WaveIDQueue, int*MobCount);

#endif
