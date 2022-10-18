#include "CProcessing.h"
#include "Mob.h"
#include <stdlib.h>
#include <stdio.h>

int Height = 800;
int Width = 800;

int MobPoolQuantity = 100;
Mob *MobPoolPtr, tempPlayer;
int waveTrack[5], cWave = 0;

int spawnInterval = 5, currentTime;

/*
Mob C Flow
: Generate N amt of mobs at random locations
: Allow Random Movement of Mobs Towards Player

Mob Cycle:
: Init -> Generate mobs as per waves count;
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
void game_init(void) {

	CP_System_SetWindowSize(Width, Height);
	MobPoolPtr = malloc(MobPoolQuantity * sizeof(Mob *));
	
	if (NULL == MobPoolPtr) {
		printf("Allocation MobPoolPtr Failed\n");
	}
	waveTrack[0] = 350;
	waveTrack[1] = 50;
	waveTrack[2] = 10;
//	int* MobQPtr = MobPoolQuantity;
	GenerateWaves(MobPoolPtr, &MobPoolQuantity, waveTrack[cWave]);
//	tempPlayer = CreateMob(2, )
}

void game_update(void) {
	
	/* Condition for spawn
		: If no more mobs, spawn interval reached
	*/
	currentTime = CP_System_GetMillis();
		
	int meh = MobPoolQuantity;

	CP_Graphics_ClearBackground(CP_Color_Create(0,0,0,255));
	for (int i = 0; i < MobPoolQuantity; i++) {
		DrawMob(&MobPoolPtr[i]);
	}
}

void game_exit(void) {
	//Free Pool
	free(MobPoolPtr);
}