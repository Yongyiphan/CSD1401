#include "cprocessing.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Camera.h"
#include "player.h"
#include "utils.h"
#include "Mob.h"

#define MAP_SIZEX 1300
#define MAP_SIZEY 900
#define PLAYER_HP 100.0f
#define PLAYER_SPEED 200.0f
#define PLAYER_DAMAGE 1.0f
#define ATK_SPEED 2.0f
#define DEFENSE 10
#define PLAYER_HITBOX 50


Player P;
CP_Vector start_vector;
CP_Color grey, black, red, green, blue, white;

//Mob Stuff
int MobPoolQuantity = 50;
WaveTrack waveTrack[1];
// pause state for the game when paused.
int isPaused;
CP_Vector centerOffSet;


void map_Init(void) {
	
	CP_System_SetWindowSize(MAP_SIZEX, MAP_SIZEY);
	//CP_System_Fullscreen();
	isPaused = 0;

	grey = CP_Color_Create(111, 111, 111, 255);
	white = CP_Color_Create(255, 255, 255, 255);
	red = CP_Color_Create(255, 0, 0, 255);
	green = CP_Color_Create(0, 255, 0, 255);
	blue = CP_Color_Create(0, 0, 255, 255);

	start_vector = CP_Vector_Zero();
	// Initialize the coordinates and stats of the player
	P = (Player){ start_vector.x, start_vector.y, 90, PLAYER_HP, PLAYER_SPEED, PLAYER_DAMAGE, ATK_SPEED, DEFENSE, PLAYER_HITBOX};
	waveTrack[0] = (WaveTrack){ 0, 0, 50, MobPoolQuantity, malloc(sizeof(Mob) * MobPoolQuantity)};
	//GenerateWaves(&waveTrack[0], 0, CP_System_GetDisplayWidth(), 0, CP_System_GetDisplayHeight(), 50);
	GenerateWaves(&waveTrack[0], 0, CP_System_GetWindowWidth(), 0, CP_System_GetWindowHeight(), 50);
	centerOffSet = (CP_Vector){ CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() / 2.0f };
	
	CameraDemo_Init();
}

void map_Update(void) {
	int* checkPause = &isPaused;
	show_healthbar(P);
	if (isPaused) {
		option_screen(checkPause);
		if (CP_Input_KeyTriggered(KEY_ESCAPE))
			isPaused = 0;
	}
	else {
		if (CP_Input_KeyTriggered(KEY_ESCAPE))
			isPaused = 1;
		if (CP_Input_KeyDown(KEY_H)) {
			P.SPEED *= 1.1;
		}

		WaveTrack *cWave = &waveTrack[0];

		
		//offsetVector = p.x, p.y
		CP_Vector offsetVector = CP_Vector_Set(P.x, P.y);
		CP_Vector offsetOrigin = CP_Vector_Subtract(offsetVector, centerOffSet);
		CP_Vector currentPosition = CP_Vector_Scale(offsetOrigin, -1.0f);
		
		printf("\n\nCurrent Pos: %f %f\n\n", P.x, P.y);
		for (int i = 0; i < cWave->MobCount; i++) {
			MobBasicAtk(&cWave->arr[i], P.x + currentPosition.x, P.y + currentPosition.y);
			DrawMob(&cWave->arr[i], 255, 255, 255);
		}
		CameraDemo_Update(&P);
	}
	
	

	if (CP_Input_KeyTriggered(KEY_SPACE))
		CP_Engine_Terminate();

	
}

void map_Exit(void) {

}