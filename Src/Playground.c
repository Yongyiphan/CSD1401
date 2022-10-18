#include "CProcessing.h"
#include "Mob.h"
#include <stdlib.h>
#include <stdio.h>

int Height = 800;
int Width = 500;

int MobPoolQuantity = 100;
Mob *MobPoolPtr;
int waveTrack[5], cWave = 0;
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

Print Conditions
: Waves with mob count > 0

*/
void game_init(void) {

	CP_System_SetWindowSize(Width, Height);
	MobPoolPtr = (Mob*) malloc(MobPoolQuantity * sizeof(Mob));
	
	if (NULL == MobPoolPtr) {
		printf("Allocation MobPoolPtr Failed\n");
	}
	waveTrack[0] = MobPoolQuantity;
	waveTrack[1] = MobPoolQuantity;
	waveTrack[2] = MobPoolQuantity;
	waveTrack[3] = MobPoolQuantity;
	waveTrack[4] = MobPoolQuantity;

	MobFactory(MobPoolPtr, MobPoolQuantity, waveTrack[cWave]);
	printf("Hello\n");

}

void game_update(void) {

	CP_Graphics_ClearBackground(CP_Color_Create(0,0,0,255));
	for (int i = 0; i < MobPoolQuantity; i++) {
		DrawMob(&MobPoolPtr[i]);
	}
}

void game_exit(void) {
	//Free Pool
	free(MobPoolPtr);
}