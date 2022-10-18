#include "CProcessing.h"
#include "Mob.h"
#include <stdlib.h>
#include <stdio.h>

int Height = 800;
int Width = 500;

int MobPoolQuantity = 100;
Mob *MobPoolPtr;
int waveTrack[5], cWave = 0;

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