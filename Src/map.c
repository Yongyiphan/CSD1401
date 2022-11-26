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
	BulletImgLoad();
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
		if (P.CURRENT_HP <= 0) {
			isDead = 1;
			isPaused = 1;
		}

#pragma endregion
		
		// Any objects below this function will be displaced by the camera movement
		CameraDemo_Update(&P, &transform);
		CP_Graphics_ClearBackground(dark_green);

		BulletDraw(); // Drawing all active bullets

		GenerateWaves();
		CP_Settings_Save();
		CP_Settings_NoFill();
		CP_Settings_StrokeWeight(0.5f);
		CP_Graphics_DrawCircle(P.x, P.y, P.STATTOTAL.PICKUP_TOTAL);
		CP_Settings_Restore();

		for (int w = 0; w < NO_WAVES; w++) {
			if (WaveIDQueue[w] == -1) {
				continue;
			}
			cWave = &WaveTracker[w];
			if (cWave->CurrentCount == 0) {
				//if all mobs are dead
				//return index to wave queue
				WaveIDQueue[w] = -1;
				//skip rest of algo
				continue;
			}
			int deadCounter = 0, alive = 0;
			for (int i = 0; i < cWave->MobCount; i++) {
				cMob = cWave->arr[i];
				//Only bother handle mobs that are alive
				//Dead = 0, Alive = 1
				if (cMob->Status == 1) {
					//MobTPlayerCollision(cMob, &P);
					cMob->AnimationCycle += 1;
					MobTMobCollision(cMob);
					MobTPlayerCollision(cMob, &P);


					int bchecker;
					// Check if current mob is colliding with a bullet, bchecker will be the collided bullet
					bchecker = BulletCollision(cMob->coor.x, cMob->coor.y, cMob->w, cMob->h);

					// Check collision of mob against the explosion radius of explosive bullet
					if (bullet[bchecker].type == PBULLET_ROCKET && bullet[bchecker].friendly == BULLET_PLAYER
						&& bullet[bchecker].exist == FALSE) // Specific type for explosion zone
					{
						// Update mob health based on bullet dmg
						cMob->CStats.HP -= bullet[bchecker].damage;
						// Mob dies if HP reaches 0 or lower
						if (cMob->CStats.HP <= 0)
							cMob->Status = 0;
					}

					// Check collision of mob against bullets
					if (bchecker >= 0 && bullet[bchecker].friendly == BULLET_PLAYER && bullet[bchecker].exist == TRUE)
					{
						// Update mob health based on bullet dmg
						cMob->CStats.HP -= bullet[bchecker].damage;
						// Mob dies if HP reaches 0 or lower
						if (cMob->CStats.HP <= 0)
							cMob->Status = 0;
						bullet[bchecker].exist = FALSE; // Bullet stop existing on collision
					}

					if (cMob->Status == 0) {
						insertItemLink(&ItemTracker->ExpLL, CreateItemEffect(cMob->coor, EXP, cMob->Title));
						float rng = CP_Random_RangeFloat(0, 1);
						if (rng < 0.33) {
							insertItemLink(&ItemTracker->ItemLL, CreateItemEffect(cMob->coor, -1, 0));
						}
						if (rng < 0.44) {
							insertItemLink(&ItemTracker->CoinLL, CreateItemEffect(cMob->coor, COIN, 0));
						}
						int sub = P.LEVEL.VAL > 0 ? P.LEVEL.VAL : 2;
						P.CURRENT_HP += sub * 2;
						cMob = &(Mob) { 0 };
						continue;
					}
					//cMob->h == 0 means haven drawn before. / assigned image to it yet
					if (P.x - WWidth / 2 - cMob->w < cMob->coor.x && cMob->coor.x < P.x + WWidth / 2 + cMob->w && P.y - WHeight / 2 - cMob->h < cMob->coor.y && cMob->coor.y < P.y + WHeight / 2 + cMob->h || cMob->h == 0) {
						DrawMobImage(cMob, &P);
					}
					alive++;
				}
				else {
					cMob = &(Mob) { 0 };
				}
				cWave->CurrentCount = alive;
			}
		}
		//PrintWaveStats();
		//PrintItemCount();
		CheckItems();
		if (MobCycleTimer % 2 == 0) {
			float deduct = 1 + P.LEVEL.VAL / 4;
			deduct = deduct > P.STATTOTAL.MAX_HP_TOTAL / 2 ? P.STATTOTAL.MAX_HP_TOTAL / 2 : deduct;
			P.CURRENT_HP -= deduct;
		}
#pragma region
		//printf("MobCount: %d |\tFPS: %f \n", MobC, CP_System_GetFrameRate());

		// Bullet CD stuff below
		float bulletangle = 0;
		// Large value so all bullet can shoot from the start with no cd
		static float bulletcd[4] = {99, 99, 99, 99};

		// Checks for mouse left button and shoot bullets when valid
		if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT))
		{
			// Update the timer for different bullet types
			UpdateCDTimer(bulletcd, 4);

			// Get mouse coord and call function point_point_angle to calculate angle from player to mouse
			mousex = CP_Input_GetMouseWorldX();
			mousey = CP_Input_GetMouseWorldY();
			bulletangle = point_point_angle(P.x, P.y, mousex, mousey);

			// Check cd valid for normal and spilt
			for (int i = 0; i < 2; i++)
			{
				// CD of 1s base value but affected by atk_speed
				if (bulletcd[i] > 1 / P.STATTOTAL.ATK_SPEED_TOTAL)
					bulletcd[i] = 0;
			}

			// Check cd valid for explosive bullet
			// CD of 3s base value but affected by atk_speed
			if (bulletcd[2] > 3 / P.STATTOTAL.ATK_SPEED_TOTAL)
				bulletcd[2] = 0;

			// Check cd valid for homing bullet
			// CD of 2s base value but affected by atk_speed
			if (bulletcd[3] > 2 / P.STATTOTAL.ATK_SPEED_TOTAL)
				bulletcd[3] = 0;

			// Default bullet is always active, no additional check required
			if (bulletcd[0] == 0) {
				BulletShoot(P.x, P.y, bulletangle, PBULLET_NORMAL, BULLET_PLAYER);
			}
			
			// Shoot other bullet types when valid
			for (int i = 1; i < 4; i++)
			{
				// Bulletlegal checks for when bullet item buff is active
				if ((Bulletlegal(i + 1) == 1) && bulletcd[i] == 0)
					BulletShoot(P.x, P.y, bulletangle, i+1, BULLET_PLAYER);
			}
			
		}

		// Keeps bulletcd running even when not on leftclick
		if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT) == FALSE)
		{
			UpdateCDTimer(bulletcd, 4);
		}

#pragma endregion

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
	Player_Show_Coins();
	show_healthbar(&P);
	show_level(&P);

	if (CP_Input_KeyTriggered(KEY_SPACE))
		CP_Engine_Terminate();
	
}

void map_Exit(void) {
	FreeMobResource();
	
	FreeItemResource();

	BulletImgFree();
	printf("Coin Gained: %d", P.STAT.Coin_Gained);
	
	//free(ItemTracker);
}
