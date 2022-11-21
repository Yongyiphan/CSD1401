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
		if (P.CURRENT_HP <= 0) {
			isDead = 1;
			isPaused = 1;
		}

#pragma endregion
		
		// Any objects below this function will be displaced by the camera movement
		CameraDemo_Update(&P, &transform);
		CP_Graphics_ClearBackground(dark_green);
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
					//static int breset = BULLET_CAP + 1; // Ensures first run value will be always be different from bchecker
					bchecker = BulletCollision(cMob->coor.x, cMob->coor.y, cMob->w, cMob->h);

					if (bullet[bchecker].type == PBULLET_ROCKET && bullet[bchecker].friendly == BULLET_PLAYER
						&& bullet[bchecker].exist == FALSE) // Specific type for explosion zone
					{
						cMob->CStats.HP -= bullet[bchecker].damage;
						if (cMob->CStats.HP <= 0)
							cMob->Status = 0;
					}

					if (bchecker >= 0 && bullet[bchecker].friendly == BULLET_PLAYER && bullet[bchecker].exist == TRUE)
					{
						cMob->CStats.HP -= bullet[bchecker].damage;
						if (cMob->CStats.HP <= 0)
							cMob->Status = 0;
						bullet[bchecker].exist = FALSE;
					}

					if (cMob->Status == 0) {
						cWave->CurrentCount -= 1;
						MobCount[w] -= 1;
						insertItemLink(&ItemTracker->ExpLL, CreateItemEffect(cMob->coor, EXP, cMob->Title));
						float rng = CP_Random_RangeFloat(0, 1);
						if (rng < 0.33) {
							insertItemLink(&ItemTracker->ItemLL, CreateItemEffect(cMob->coor, -1, 0));
						}
						if (rng < 0.44) {
							insertItemLink(&ItemTracker->CoinLL, CreateItemEffect(cMob->coor, COIN, 0));
						}
						int sub = P.LEVEL.VAL > 0 ? P.LEVEL.VAL : 2;
						P.CURRENT_HP += sub;
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
		//PrintItemCount();
		CheckItems();
		if (MobCycleTimer % 2 == 0) {
			float deduct = 1 + P.LEVEL.VAL / 4;
			deduct = deduct > P.STATTOTAL.MAX_HP_TOTAL * 2 / 3  ? P.STATTOTAL.MAX_HP_TOTAL * 2 / 3 : deduct;
			//P.CURRENT_HP -= deduct;
		}
#pragma region
		static float bulletcd = 99; // Random big number so no cd on first shot
		static btype = 2;

		//printf("MobCount: %d |\tFPS: %f \n", MobC, CP_System_GetFrameRate());

		// Bullet CD Related stuff below
		float bulletangle = 0;
		static float bulletcd1 = 99, bulletcd2 = 99, bulletcd3 = 99, bulletcd4 = 99; // Random big number so no cd on first shot
		static int legal2 = 0, legal3 = 0, legal4 = 0; // Manual overwrite for bullet types, for testing use
		if (CP_Input_KeyTriggered(KEY_1)) // For testing, keypad 1 to toggle on / off
		{
			if (legal2 == 0)
				legal2 = 1;
			else
				legal2 = 0;
		}
		if (CP_Input_KeyTriggered(KEY_2)) // For testing, keypad 2 to toggle on / off
		{
			if (legal3 == 0)
				legal3 = 1;
			else
				legal3 = 0;
		}
		if (CP_Input_KeyTriggered(KEY_3)) // For testing, keypad 3 to toggle on / off
		{
			if (legal4 == 0)
				legal4 = 1;
			else
				legal4 = 0;
		}
		if (CP_Input_KeyTriggered(KEY_4)) // Toggle all types on / off
		{
			if (legal2 == 0 || legal3 == 0 || legal4 == 0)
				legal2 = legal3 = legal4 = 1;
			else
				legal2 = legal3 = legal4 = 0;
		}

		if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT))
		{
			// Get details for bullets
			bulletcd1 += CP_System_GetDt();
			bulletcd2 += CP_System_GetDt();
			bulletcd3 += CP_System_GetDt();
			bulletcd4 += CP_System_GetDt();
			mousex = CP_Input_GetMouseWorldX();
			mousey = CP_Input_GetMouseWorldY();
			bulletangle = point_point_angle(P.x, P.y, mousex, mousey);

			// Check valid cd + shoot for each bullet type
			if (bulletcd1 > 1 / P.STATTOTAL.ATK_SPEED_TOTAL) { // Fixed value is the base cd timer
				bulletcd1 = 0;
			}
			if (bulletcd1 == 0) {
				// Default bullet is always active
				BulletShoot(P.x, P.y, bulletangle, PBULLET_NORMAL, BULLET_PLAYER);
			}

			if (bulletcd2 > 1 / P.STATTOTAL.ATK_SPEED_TOTAL) { // Fixed value is the base cd timer
				bulletcd2 = 0;
			}
			if ((Bulletlegal(2) == 1 || legal2 == 1) && bulletcd2 == 0)
				BulletShoot(P.x, P.y, bulletangle, PBULLET_SPILT, BULLET_PLAYER);

			if (bulletcd3 > 3 / P.STATTOTAL.ATK_SPEED_TOTAL) { // Fixed value is the base cd timer
				bulletcd3 = 0;
			}
			if ((Bulletlegal(3) == 1 || legal3 == 1) && bulletcd3 == 0)
				BulletShoot(P.x, P.y, bulletangle, PBULLET_ROCKET, BULLET_PLAYER);

			if (bulletcd4 > 2 / P.STATTOTAL.ATK_SPEED_TOTAL) { // Fixed value is the base cd timer
				bulletcd4 = 0;
			}
			if ((Bulletlegal(4) == 1 || legal4 == 1) && bulletcd4 == 0)
				BulletShoot(P.x, P.y, bulletangle, PBULLET_HOMING, BULLET_PLAYER);

		}
		if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT) == FALSE && bulletcd1 != 99) // Keeps bulletcd running even when not on leftclick
		{
			bulletcd1 += CP_System_GetDt();
			if (bulletcd1 > 0.5)
				bulletcd1 = 99;
		}
		if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT) == FALSE && bulletcd2 != 99) // Keeps bulletcd running even when not on leftclick
		{
			bulletcd2 += CP_System_GetDt();
			if (bulletcd2 > 0.5)
				bulletcd2 = 99;
		}
		if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT) == FALSE && bulletcd3 != 99) // Keeps bulletcd running even when not on leftclick
		{
			bulletcd3 += CP_System_GetDt();
			if (bulletcd3 > 0.5)
				bulletcd3 = 99;
		}
		if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT) == FALSE && bulletcd4 != 99) // Keeps bulletcd running even when not on leftclick
		{
			bulletcd4 += CP_System_GetDt();
			if (bulletcd4 > 0.5)
				bulletcd4 = 99;
		}

		BulletDraw();

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
	printf("Coin Gained: %d", P.STAT.Coin_Gained);
	
	//free(ItemTracker);
}
