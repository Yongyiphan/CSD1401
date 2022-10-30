#include "map.h"
#include "cprocessing.h"
#include <stdlib.h>
#include <stdio.h>
#include "Camera.h"
#include "player.h"
#include "utils.h"
#include "Mob.h"
#include "bullet.h"
#include "Items.h"


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
#define NO_WAVES 6
#define Spawn_Timer 1
#define Wave_Timer 5
#define SpawnAreaOffset 300

Mob* cMob;
int StartMobQuantity = 100, cWaveID = 0,currentWaveCost, MaxMob;
int currentSec = 0;
int WaveIDQueue[NO_WAVES];
WaveTrack waveTrack[NO_WAVES], *cWave; // pause state for the game when paused.

//Might be useful variable for Waves Tracking
int totalWave = 0, MobCount[NO_WAVES];
int isPaused;
float mousex, mousey;

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
	currentWaveCost = 10;
	MaxMob = 200;
	for (int i = 0; i < NO_WAVES; i++) {
		waveTrack[i] = (WaveTrack){
			MaxMob,
			0,
			0,
			0, 
			malloc(sizeof(Mob*) * StartMobQuantity), 
			StartMobQuantity, 
			SpawnAreaOffset,
			white
		};
		InitWavesArr(&waveTrack[i]);
		WaveIDQueue[i] = -1;
	}
	CameraDemo_Init();
	Bulletinit();
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
			//Every SpawnTime interval spawn wave
			if (currentSec % Wave_Timer == 0) {
				//Growth Per Wave
				MaxMob += 50;
				//printf("Max Mobs Increased to %d\n", MaxMob);
			}
			if (currentSec % Spawn_Timer == 0) {
				//Growth Per Wave
				currentWaveCost += 20;
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
				GenerateWaves(&P, &waveTrack, &WaveIDQueue, NO_WAVES, currentWaveCost, MaxMob, &totalWave, &MobCount);
				//Used to print current wave statistics, can be removed :)
				PrintWaveStats(&totalWave,NO_WAVES, &WaveIDQueue, &MobCount);
			}
		}

		int MobC = 0;
		for (int w = 0; w < NO_WAVES; w++) {
			cWave = &waveTrack[w];
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
					MobC += 1;
					MobPathFinding(cMob, P.x, P.y);
					MobCollision(cMob, &P);
					int bchecker;
					if ((bchecker = BulletCollision(cMob->coor.x, cMob->coor.y, cMob->CStats.size)) >= 0 && bullet[bchecker].friendly == BULLET_PLAYER)
					{
						cMob->CStats.HP -= bullet[bchecker].damage;
						if (cMob->CStats.HP <= 0)
							cMob->Status = 0;
						bullet[bchecker].exist = FALSE;
					}
					if (cMob->Status == 0) {
						cWave->CurrentCount -= 1;
						MobCount[w] -= 1;
						continue;
					}
					DrawMob(cMob, cWave->waveColor.r, cWave->waveColor.g, cWave->waveColor.b);
				}
			}
		}
		//printf("MobCount: %d |\tFPS: %f \n", MobC, CP_System_GetFrameRate());
		
		if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT))
		{
			mousex = CP_Input_GetMouseWorldX();
			mousey = CP_Input_GetMouseWorldY();
			float bulletangle = 0;
			bulletangle = point_point_angle(P.x, P.y, mousex, mousey);
			BulletShoot(P.x, P.y, bulletangle, 1, BULLET_PLAYER);
		}
		BulletDraw();
	}
	

	if (CP_Input_KeyTriggered(KEY_SPACE))
		CP_Engine_Terminate();

	
}

void map_Exit(void) {
	for (int i = 0; i < NO_WAVES; i++) {
		free(waveTrack[i].arr);
	}
}