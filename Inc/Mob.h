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
	float x, y;
	int Status; 
	int AnimationCycle;
	int w, h;
	//Dead = 0 | Alive = 1
} Mob;

typedef struct Node
{
    Mob* key;
    struct MobNode *left;
    struct MobNode *right;
    int h;
}MobNode;
 

typedef struct WaveTracker {
	int MaxMob;
	int MobCount;
	int CurrentCount;
	int WaveCost;
	int arrSize;
	int spawnOffset;
	Mob** arr;
	MobNode* tree;
}WaveTrack;

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

int treeheight(MobNode* N);
int getBalance(MobNode* N);

MobNode* newNode(Mob mob);
MobNode* insert(MobNode* node, Mob key);
MobNode* rightRotate(Mob* m);
MobNode* leftRotate(Mob* m);

MobNode* FindDeadMob(MobNode* N);

float getX(MobNode* N);
void freeTree(MobNode* node);


void MobLoadImage(CP_Image *Sprites, int No_Img);
void InitWavesArr(WaveTrack* tracker, int start);

void CreateBaseStat(MobStats* ms, int type);
void CreateMob(Mob* m, MobStats *Base, Player*player, int offSet);
void GenerateMobs(WaveTrack* tracker, Player* player);
void GenerateWaves(Player* P, WaveTrack* queue, int* queueID, int WavesNo, int CostGrowth, int MaxMobGrowth,int *TotalWaveCount,  int* MobCount);

void DrawMobImage(CP_Image* Sprites, Mob* m, Player*p);


void MobTPlayerCollision(Mob* m, Player* p);
void MobTMobCollision(Mob* mob, Player* p, WaveTrack* tracker, int const No_Waves);

void PrintWaveStats(int*CWaveCount, int No_Waves, int*WaveIDQueue, int*MobCount);

void FreeMobResource(WaveTrack* wtracker, int noWaves, CP_Image *spritesheet, int Mob_Img);

float square(float one, float two);














#endif
