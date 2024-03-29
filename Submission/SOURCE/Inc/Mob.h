#pragma once
#ifndef MOB_H
#define MOB_H

/* --------------------------------------
* File Level Documentation
* @author	Edgar Yong Yip Han
* @email	y.yiphanedgar.digipen.edu
* @file		Mob.h
* @brief	This file contains all functions required for Mob Spawning during game runtime.
			-> Initialises, generate mob objects, draw mob objects
			-> Pathing for different mobs
			-> Mob and player iteraction
			-> Free All resources created through malloc

* Copyright 2022 Digipen, All Rights Reserved.
*//*-------------------------------------*/

#include "player.h"
#include "cprocessing.h"

#define MobTypes 2
#define SmallMob 0
#define MediumMob 1



typedef struct MobBase {
	float HP;
	float DEF;
	float Speed;

	float Range;
	float Dmg;
	float size;

}MobStats;

typedef struct Mob {
	int Title;

	MobStats BaseStats;
	MobStats CStats;
	//float x, y;
	CP_Vector coor, dest;
	int Status; 
	int AnimationCycle;
	float w, h;
	//Dead = 0 | Alive = 1
	int boost, jump;
} Mob;

static const struct Mob EmptyMob;
 

typedef struct WaveTracker {
	int MaxMob;
	int MobCount;
	int CurrentCount;
	int WaveCost;
	int arrSize;
	int spawnOffset;
	int Start;
	Mob** arr;
}WaveTrack;

//Mob Image
#define Img_Scale 80
#define BoundScale 1.2f

//extern CP_Image** MobSprites;
#define MSpriteSize 5
extern CP_Image MobSprites[MSpriteSize];
extern int Mob_Img;
// --------------------------- //

//Mob Stuff
#define NO_WAVES 5
#define Spawn_Timer 5
#define Wave_Timer 30
#define MaxMobGrowthRate 30
#define MaxUpperLimit 1000
#define WaveCostGrowthRate 2
#define SpawnAreaOffset 1000
#define StartMobQuantity 1500
#define MobStatScale 1.04f
#define BIGNONO 180

extern int WaveIDQueue[NO_WAVES], MobCount[NO_WAVES], CWave, CWaveCost, CMaxMob;
extern WaveTrack WaveTracker[NO_WAVES];
extern int MobCycleTimer;

#define SWAP(one, two) (one ^= two ^= one ^=two)

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
void GenerateWaves(void);

void DrawMobImage(Mob* m, Player*p);

void MobTPlayerCollision(Mob* m, Player* p);
void MobMovement(Mob* mob);

void PrintWaveStats(void);

void FreeMobResource(void);

float squareDist(float one, float two);



#endif








