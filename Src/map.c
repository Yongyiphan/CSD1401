#pragma once
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



int WHeight, WWidth;
float sfxVolume, bgmVolume;

CP_Color dark_green;
CP_Matrix transform;

Mob* cMob;
WaveTrack *cWave; // pause state for the game when paused.

//Might be useful variable for Waves Tracking
int totalWave = 0;
float mousex, mousey;
int isPaused, isUpgrade, isDead;

//Images
CP_Image background = NULL;

void map_Init(void) {
	//CP_System_SetFrameRate(60);
	CP_System_SetWindowSize(MAP_SIZEX, MAP_SIZEY);
	WHeight = CP_System_GetWindowHeight();
	WWidth = CP_System_GetWindowWidth();
	//CP_System_Fullscreen();
	isPaused = 0, isUpgrade = 0, isDead = 0;
	sfxVolume = 0.7, bgmVolume = 0.7;

	// initialize the timer to start from 0 
	timer(1, isPaused);

	background = CP_Image_Load("./Assets/background.png");
	dark_green = CP_Color_Create(50, 50, 0, 255);
	CP_Graphics_ClearBackground(dark_green);
	// Initialize the coordinates and stats of the player

	
	CreateWaveTracker();
	CreateItemTracker();
	MobLoadImage();
	ItemLoadImage();
	Player_Init(&P);
	CameraDemo_Init();
	Bulletinit();
}

void map_Update(void) {
	
	// Update player stats, inclusive of base stats and multipliers.
	Player_Stats_Update(&P);
#pragma region	
	if (isPaused) {
		// Opens up the Upgrade Screen for players to pick their upgrades
		if (isUpgrade) {
			upgrade_screen(&P, &isUpgrade, &isPaused);
			//printf("Player max hp: %f\n", P.MAX_HP);
		}
		// Opens up the Pause Screen
		else if (P.CURRENT_HP > 0) {
			option_screen(&isPaused);
		}
		// temporarily paused the death_screen function to allow the game to continue running
		if (isDead) {
			//float elapsedTime = timer(0);
			death_screen(timer(0, isDead));
		}
	
		// Resume the game
		if (CP_Input_KeyTriggered(KEY_ESCAPE))
			isPaused = 0;
	}
	// if game is not paused
	else {
		if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
			isPaused = 1;
			isUpgrade = 0;
		}
		// Increase speed of the player
		//if (CP_Input_KeyTriggered(KEY_H)){
		//	P.STATMULT.SPEED_MULT /= 1.1f;
		//}
		//if (CP_Input_KeyTriggered(KEY_M)){
		//	P.STATMULT.PICKUP_MULT *= 1.1;
		//}
		// Open up the Upgrade Screen
		/*if (CP_Input_KeyTriggered(KEY_U) && isUpgrade == 0) {
			isUpgrade = 1;
			isPaused = 1;
		}*/
		// Testing for leveling up
		if (CP_Input_KeyDown(KEY_L)) {
			P.LEVEL.P_EXP += 5;
			level_up(&P.LEVEL);
		}
		// Manually control the HP of the player for testing
		if (CP_Input_KeyDown(KEY_Q)) {
			P.CURRENT_HP -= 4;
		}
		else if (CP_Input_KeyDown(KEY_E)) {
			P.CURRENT_HP += 4;
		}
		if (P.CURRENT_HP <= 0) {
			isDead = 1;
			isPaused = 1;
		}

#pragma endregion
		// Any objects below this function will be displaced by the camera movement
		CameraDemo_Update(&P, &transform);
		GenerateWaves();
		CP_Settings_NoFill();
		CP_Graphics_DrawCircle(P.x, P.y, P.STATTOTAL.PICKUP_TOTAL);
		for (int w = 0; w < NO_WAVES; w++) {
			if (WaveIDQueue[w] == -1) {
				continue;
			}
			cWave = &WaveTracker[w];
			if (cWave->CurrentCount == 0 || (MobCycleTimer % 3 == 0 && cWave->CurrentCount == 1)) {
				//if all mobs are dead
				//return index to wave queue
				WaveIDQueue[w] = -1;
				//skip rest of algo
				continue;
			}
			for (int i = 0; i < cWave->MobCount; i++) {
				cMob = cWave->arr[i];
				//Only bother handle mobs that are alive
				//Dead = 0, Alive = 1
				if (cMob->Status == 1) {
					//MobTPlayerCollision(cMob, &P);
					MobTMobCollision(cMob);
					MobTPlayerCollision(cMob, &P);
					int bchecker;
					if ((bchecker = BulletCollision(cMob->coor.x, cMob->coor.y, cMob->w, cMob->h)) >= 0 && bullet[bchecker].friendly == BULLET_PLAYER
						&& bullet[bchecker].exist == TRUE)
					{
						cMob->CStats.HP -= bullet[bchecker].damage;
						if (cMob->CStats.HP <= 0)
							cMob->Status = 0;
						bullet[bchecker].exist = FALSE;
					}

					if (cMob->Status == 0) {
						cWave->CurrentCount -= 1;
						MobCount[w] -= 1;
						//ItemTracker->exptree = insertItemNode(ItemTracker->exptree, CreateItemEffect(cMob->x, cMob->y, 1, cMob->Title));
						//insertItemLink(&ItemTracker->ExpLL, CreateItemEffect(cMob->coor, EXP, cMob->Title));
						float rng = CP_Random_RangeFloat(0, 1);
						if (rng < 0.83) {
							insertItemLink(&ItemTracker->ItemLL, CreateItemEffect(cMob->coor, -1, 0));
						}
						if (rng < 0.44) {
							//insertItemLink(&ItemTracker->CoinLL, CreateItemEffect(cMob->coor, COIN, 0));
						}
						int sub = P.LEVEL.VAL > 0 ? P.LEVEL.VAL : 2;
						P.CURRENT_HP += sub / 2;
						continue;
					}
					//cMob->h == 0 means haven drawn before. / assigned image to it yet
					if (P.x - WWidth/2 - cMob->w < cMob->coor.x && cMob->coor.x < P.x + WWidth/2 + cMob->w && P.y - WHeight/2 - cMob->h < cMob->coor.y && cMob->coor.y < P.y + WHeight/2 + cMob->h || cMob->h == 0) {
						DrawMobImage(cMob, &P);
					}
				}
				
			}
		}
		if (ItemTracker->ItemLL != NULL) {
			ItemTracker->ItemLL = ItemInteraction(ItemTracker->ItemLL);
		}
		if (ItemTracker->ExpLL != NULL) {
			ItemTracker->ExpLL = ItemInteraction(ItemTracker->ExpLL);
		}
		if (ItemTracker->CoinLL != NULL) {
			ItemTracker->CoinLL = ItemInteraction(ItemTracker->CoinLL);
		}

		static float bulletcd = 99; // Random big number so no cd on first shot
		static btype = 2;
		if (CP_Input_KeyTriggered(KEY_1)) // For testing, keypad 1 to switch to spilt, if spilt then to normal
		{
			if (btype == PBULLET_SPILT) btype = PBULLET_NORMAL;
			else btype = PBULLET_SPILT;
		}
		if (CP_Input_KeyTriggered(KEY_2)) // For testing, keypad 2 to switch to rocket, if spilt then to normal
		{
			if (btype == PBULLET_ROCKET) btype = PBULLET_NORMAL;
			else btype = PBULLET_ROCKET;
		}
		if (CP_Input_KeyTriggered(KEY_3)) // For testing, keypad 3 to switch to rocket, if spilt then to normal
		{
			if (btype == PBULLET_HOMING) {
				printf("Swap homing\n");  btype = PBULLET_HOMING;
			}
			else btype = PBULLET_HOMING;
		}
		if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT))
		{
			bulletcd += CP_System_GetDt();
			if (bulletcd > 0.5) { // 0.5 is the cooldown timer; might need seperate timer for different bullet types
				bulletcd = 0;
			}
			mousex = CP_Input_GetMouseWorldX();
			mousey = CP_Input_GetMouseWorldY();
			if (bulletcd == 0) {
				float bulletangle = 0;
				bulletangle = point_point_angle(P.x, P.y, mousex, mousey);
				BulletShoot(P.x, P.y, bulletangle, btype, BULLET_PLAYER);
			}
		}
		if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT) == FALSE && bulletcd != 99) // Keeps bulletcd running even when not on leftclick
		{
			bulletcd += CP_System_GetDt();
			if (bulletcd > 0.5)
				bulletcd = 99;
		}

		
			
		BulletDraw();
		UpdateAppliedEffects(NULL);
		DrawAppliedEffects();
		CP_Settings_ResetMatrix();
		// Time, returns and draws text
		timer(0, isPaused);
	}

	// Shows the upgrade screen whenever the player levels up.
	if (level_up(&P.LEVEL)) {
		isPaused = 1;
		isUpgrade = 1;
		upgrade_screen(&P, &isUpgrade, &isPaused);
	}
	Player_Show_Stats(P);
	show_healthbar(&P);
	show_level(&P);

	if (CP_Input_KeyTriggered(KEY_SPACE))
		CP_Engine_Terminate();
	CP_Graphics_ClearBackground(dark_green);
}

void map_Exit(void) {
	FreeMobResource();
	
	FreeItemResource();
	printf("Coin Gained: %d", P.STAT.Coin_Gained);
	
	//free(ItemTracker);
}
