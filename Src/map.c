#include "map.h"
#include "cprocessing.h"
#include <stdlib.h>
#include <stdio.h>
#include "Camera.h"
#include "player.h"
#include "utils.h"
#include "Mob.h"
#include "Items.h"


#define MAP_SIZEX 1300
#define MAP_SIZEY 900
#define PLAYER_HP 100.0f
#define PLAYER_SPEED 200.0f
#define PLAYER_DAMAGE 1.0f
#define ATK_SPEED 2.0f
#define DEFENSE 10
#define PLAYER_HITBOX 50

//Sprite Stuff
#define Mob_Img 4
CP_Image** MobSprites;

Player P;
CP_Vector start_vector;
CP_Color grey, black, red, green, blue, white;

//Starting Quantity
int StartMobQuantity = 10, StartItemQuantity = 1000;

//Mob Stuff
#define NO_WAVES 4
#define Spawn_Timer 1
#define Wave_Timer 5
#define MaxMobGrowthRate 30
#define WaveCostGrowthRate 10
#define SpawnAreaOffset 50

Mob* cMob;
int cWaveCost, MaxMob;
int currentSec = 0;
int WaveIDQueue[NO_WAVES];
WaveTrack WaveTracker[NO_WAVES], *cWave; // pause state for the game when paused.

//Might be useful variable for Waves Tracking
int totalWave = 0, MobCount[NO_WAVES];

//Item Stuff
//ItemTrack *ItemTracker;

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
	P = (Player){ start_vector.x, start_vector.y, 90, PLAYER_HP, PLAYER_SPEED, PLAYER_DAMAGE, ATK_SPEED, DEFENSE, PLAYER_HITBOX}; //Initialise empty arrays of possible waves
	cWaveCost = WaveCostGrowthRate;
	MaxMob = MaxMobGrowthRate;
	for (int i = 0; i < NO_WAVES; i++) {
		WaveTracker[i] = (WaveTrack){
			MaxMob, //Max Mob
			0, //MobCount
			0, //Current Mob Count
			0, //Wave Cost
			StartMobQuantity, //Array Size 
			SpawnAreaOffset, //Spawn offset
			malloc(sizeof(Mob*) * StartMobQuantity) }; //, //Arr
	//		white //Wave Color
	//	};
		InitWavesArr(&WaveTracker[i], 0);
		WaveIDQueue[i] = -1;
	}
	//ItemTracker = &(ItemTrack){malloc(sizeof(Item*) * StartItemQuantity), StartItemQuantity, 0};
	//InitItemArr(ItemTracker);
	MobSprites = malloc(sizeof(CP_Image*) * Mob_Img);
	MobLoadImage(MobSprites, Mob_Img);
	
	
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
			P.SPEED *= 1.1f;
		}
		CameraDemo_Update(&P);
		if ((int)CP_System_GetSeconds() != currentSec) {
			currentSec = (int)CP_System_GetSeconds();
			printf("\n\tCurrent Sec: %d | Current FPS:%f\n", currentSec, CP_System_GetFrameRate());
			printf("\tMax Mob: %d | Current Cost: %d\n", MaxMob, cWaveCost);
			//Every SpawnTime interval spawn wave
			if (currentSec % Wave_Timer == 0) {
				//Growth Per Wave
				MaxMob += MaxMobGrowthRate;
				//printf("Max Mobs Increased to %d\n", MaxMob);
			}
			if (currentSec % Spawn_Timer == 0) {
				//Growth Per Wave
				cWaveCost += WaveCostGrowthRate;
				/*
				Generate Waves
				-) Update/ Reference == Require Pointers
				===== Params ================
					-> Player (Reference)
					-> WaveTrack Arr (Update)
					-> WaveID Queue (Update)
					-> No of spawnable Waves
					-> WaveCost Growth
					-> MaxMob Growth
				===== Optional =============== (May be deleted accordingly if unnecessary)
					-> Total Wave Count (Update)
					-> Mob Count (Update)
				*/
				GenerateWaves(&P, &WaveTracker, &WaveIDQueue, NO_WAVES, cWaveCost, MaxMob, &totalWave, &MobCount);
				//Used to print current wave statistics, can be removed :)
				PrintWaveStats(&totalWave,NO_WAVES, &WaveIDQueue, &MobCount);
			}
		}

		for (int w = 0; w < NO_WAVES; w++) {
			cWave = &WaveTracker[w];
			if (WaveIDQueue[w] != -1) {
				if (cWave->CurrentCount == 0) {
					//if all mobs are dead
					//return index to wave queue
					WaveIDQueue[w] = -1;
					//skip rest of algo
					continue;
				}
				//printf("Spawning wave: %d\n", w);
				for (int i = 0; i < cWave->MobCount; i++) {
					cMob = cWave->arr[i];
					//Only bother handle mobs that are alive
					//Dead = 0, Alive = 1
					if (cMob->Status == 0) {
						continue;
					}
					MobTMobCollision(cMob, &P, &WaveTracker, NO_WAVES);
					MobTPlayerCollision(cMob, &P);
					//MobCollision(cMob, &P);
					//MobPathFinding(cMob, P.x, P.y);

					if (cMob->Status == 0) {
						cWave->CurrentCount -= 1;
						MobCount[w] -= 1;
						continue;
					}
					DrawMobImage(MobSprites, cMob, &P);
				}
			}
		}
	}
	

	if (CP_Input_KeyTriggered(KEY_SPACE))
		CP_Engine_Terminate();

	
}

void map_Exit(void) {
	FreeMobResource(&WaveTracker, NO_WAVES, MobSprites, Mob_Img);
	free(MobSprites);
	
	//FreeItemResource(&ItemTracker);
	//free(ItemTracker);
}