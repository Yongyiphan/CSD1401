#include "cprocessing.h"
#include "Mob.h"
#include <stdlib.h>
#include <stdio.h>

int Height = 800;
int Width = 800;

int MobPoolQuantity = 5;
Mob *MobPoolPtr = NULL, tempPlayer;
WaveTrack waveTrack[1];
int cWave = 0, waveBuffer = 1;

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
	waveTrack[0] = (WaveTrack){ 0, 0, 7, MobPoolQuantity, malloc(sizeof(Mob) * MobPoolQuantity)};

	
//	int i = sizeof(Mob), j = sizeof(Mob*), k = sizeof(MobPoolPtr+1);
	//GenerateWaves(waveTrack[0].arr, &waveTrack[0].arrSize, waveTrack[0].waveCost, &waveTrack[0].MobCount);
	GenerateWaves(&waveTrack[0]);
	waveTrack[0].CurrentCount = waveTrack[0].MobCount;

}

void game_update(void) {
	
	/* Condition for spawn
		: If no more mobs, spawn interval reached
	*/
	currentTime = CP_System_GetMillis();
	printf("Fuck\n");
}

void game_exit(void) {
	//Free Pool
	//free(MobPoolPtr);
}