#include "cprocessing.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Camera.h"
#include "player.h"
#include "utils.h"
#include "Mob.h"
#include "bullet.h"

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
#define NO_WAVES 5
#define Spawn_Timer 2
#define SpawnAreaOffset 550

int StartMobQuantity = 1000, cWaveID = 0,currentWaveCost = 50;
int currentSec;
int WaveIDQueue[NO_WAVES], totalWave = 0;
WaveTrack waveTrack[NO_WAVES], *cWave; // pause state for the game when paused.
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
	P = (Player){ start_vector.x, start_vector.y, 90, PLAYER_HP, PLAYER_SPEED, PLAYER_DAMAGE, ATK_SPEED, DEFENSE, PLAYER_HITBOX};
	//Initialise empty arrays of possible waves
	for (int i = 0; i < NO_WAVES; i++) {
		waveTrack[i] = (WaveTrack){ 0, 0, 0, StartMobQuantity, malloc(sizeof(Mob) * StartMobQuantity), SpawnAreaOffset};
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
			P.SPEED *= 1.1;
		}
	
		CameraDemo_Update(&P);
		if ((int)CP_System_GetSeconds() != currentSec) {
			currentSec = (int)CP_System_GetSeconds();
			//Every SpawnTime interval spawn wave
			//printf("Generating Wave\n");
			if (currentSec % Spawn_Timer == 0) {
				//printf("Queue: %d %d %d %d\n", WaveIDQueue[0], WaveIDQueue[1], WaveIDQueue[2], WaveIDQueue[3]);
				for (int i = 0; i < NO_WAVES; i++) {
					//At defauly WaveIDQueue = {-1,-1,-1,-1}
					//Whereby each "-1" == to available slot to generate waves
					if (WaveIDQueue[i] == -1) {
						totalWave += 1;
						waveTrack[i].waveCost = currentWaveCost;
						//Generate Waves at avaiable slot 
						GenerateWaves(&waveTrack[i], &P);
						//Edit increment to spawn more mob each waves
						currentWaveCost += 150;
						
						WaveIDQueue[i] = totalWave;
						int r = CP_Random_RangeInt(0, 2);
						//Assign random color to each wave
						switch (r) {
						case 0:
							waveTrack[i].waveColor = red;
							break;
						case 1:
							waveTrack[i].waveColor = green;
							break;
						case 2:
							waveTrack[i].waveColor = blue;
							break;
						}
						//printf("Created wave at %d\n", 1);
						break;
					}
				}
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
					Mob* cMob = &cWave->arr[i];
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

}