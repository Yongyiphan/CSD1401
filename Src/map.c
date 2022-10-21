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
#define NO_WAVES 2
#define Spawn_Timer 5
int StartMobQuantity = 50, cWaveID = 0;
int currentTime;
int WaveIDQueue[NO_WAVES];
WaveTrack waveTrack[NO_WAVES], *cWave; // pause state for the game when paused.
int isPaused;


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
	for (int i = 0; i < NO_WAVES; i++) {
		waveTrack[i] = (WaveTrack){ 0, 0, 50, StartMobQuantity, malloc(sizeof(Mob) * StartMobQuantity)};
	}
	//GenerateWaves(&waveTrack[0], 0, CP_System_GetDisplayWidth(), 0, CP_System_GetDisplayHeight(), 50);
	GenerateWaves(&waveTrack[cWaveID], P.x, P.y, 550);
	cWave = &waveTrack[cWaveID];
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
	

		//WaveTrack *cWave = &waveTrack[cWaveID];
		
		CameraDemo_Update(&P);

		currentTime = CP_System_GetFrameCount();
		//if (currentTime % (Spawn_Timer * 5) == 0) {

		int t = (cWaveID + 1) % NO_WAVES;
		if (currentTime % (Spawn_Timer * (int) CP_System_GetFrameRate()) == 0  || currentTime == 0)  {
			printf("\n%d\n", currentTime);
			printf("%f", CP_System_GetFrameRate());
			
			GenerateWaves(&waveTrack[t], P.x, P.y, 550);
			cWaveID = t;
			cWave = &waveTrack[cWaveID];
		}

		printf("Current Wave: %d Seconds: %f\n",t, CP_System_GetSeconds());
		


		for (int i = 0; i < cWave->MobCount; i++) {
			MobPathFinding(&cWave->arr[i], P.x, P.y);
			DrawMob(&cWave->arr[i], 255, 255, 255);
		}
	}
	
	

	if (CP_Input_KeyTriggered(KEY_SPACE))
		CP_Engine_Terminate();

	
}

void map_Exit(void) {

}