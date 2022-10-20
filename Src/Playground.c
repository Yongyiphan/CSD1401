#include "cprocessing.h"
#include "Mob.h"
#include <stdlib.h>
#include <stdio.h>

int Height = 800;
int Width = 800;

int MobPoolQuantity = 50;
Mob tempPlayer;
WaveTrack waveTrack[1];
int cWaveID = 0, waveBuffer = 1;

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
	waveTrack[0] = (WaveTrack){ 0, 0, 50, MobPoolQuantity, malloc(sizeof(Mob) * MobPoolQuantity)};

	GenerateWaves(&waveTrack[0], 0, Width, 0, Height, 50);
	//waveTrack[0].CurrentCount = waveTrack[0].MobCount;
	
	//Temp player
	tempPlayer = CreateMob(3, CreateBaseStat(3), Width / 2, Width / 2, Height / 2, Height / 2, 0);
}

void game_update(void) {
	
	/* Condition for spawn
		: If no more mobs, spawn interval reached
	*/
	currentTime = CP_System_GetMillis();
	CP_Graphics_ClearBackground(CP_Color_Create(150, 150, 150, 255));	
	//Loop and Draw
	WaveTrack *cWave = &waveTrack[0];
	DrawMob(&(tempPlayer), 100, 100, 100);
	for (int i = 0; i < cWave->MobCount; i++) {
		MobBasicAtk(&cWave->arr[i], tempPlayer.coor.x, tempPlayer.coor.y);
		DrawMob(&cWave->arr[i], 255, 255, 255);
	}


}

void game_exit(void) {
	//Free Pool
	//free(MobPoolPtr);
}