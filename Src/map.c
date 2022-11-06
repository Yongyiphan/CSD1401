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
#define PLAYER_HP 100.0f
#define PLAYER_SPEED 300.0f
#define PLAYER_DAMAGE 1.0f
#define ATK_SPEED 2.0f
#define PLAYER_DEFENSE 10
#define PLAYER_HITBOX 50

int WHeight, WWidth;

//Declaring player variables
Player P; Stats P_stats; StatsMult P_stats_mult; StatsTotal P_stats_total; LEVEL level;


CP_Vector start_vector;
CP_Color grey, black, red, green, blue, white;
CP_Matrix transform;


Mob* cMob;
WaveTrack *cWave; // pause state for the game when paused.

//Might be useful variable for Waves Tracking
int totalWave = 0;
float mousex, mousey;
int isPaused, isMenu, isDead;


//Images
CP_Image background = NULL;

void map_Init(void) {
	//CP_System_SetFrameRate(60);
	CP_System_SetWindowSize(MAP_SIZEX, MAP_SIZEY);
	WHeight = CP_System_GetWindowHeight();
	WWidth = CP_System_GetWindowWidth();
	//CP_System_Fullscreen();
	isPaused = 0;
	isMenu = 0;
	isDead = 0;
	// initialize the timer to start from 0 
	timer(1, isPaused);


	grey = CP_Color_Create(111, 111, 111, 255);
	white = CP_Color_Create(255, 255, 255, 255);
	red = CP_Color_Create(255, 0, 0, 255);
	green = CP_Color_Create(0, 255, 0, 255);
	blue = CP_Color_Create(0, 0, 255, 255);

	background = CP_Image_Load("./Assets/background.png");

	start_vector = CP_Vector_Zero();
	// Initialize the coordinates and stats of the player

	/*
	P_stats: Base stats of the player, can only be altered outside of the game.
	P_stats_mult: In-game stats upgrade. Player stats are increased by multiplying its value against the player base stats
	P_stats_total: The total amount of stats by multiplying its value with player base stats.
	E.G. P_stats's MAX_HP = 100, P_stats_mult's MAX_HP_MULT = 1.2
	P_stats_total's MAX_HP_TOTAL = MAX_HP * MAX_HP_MULT
								 = 100 * 1.2 = 120
	*/
	P_stats = (Stats){ PLAYER_HP, PLAYER_SPEED, PLAYER_DAMAGE, ATK_SPEED, PLAYER_DEFENSE };
	P_stats_mult = (StatsMult){ 1, 1, 1, 1, 1 };
	P_stats_total = (StatsTotal){ PLAYER_HP, PLAYER_SPEED, PLAYER_DAMAGE, ATK_SPEED, PLAYER_DEFENSE };
	level = (LEVEL){ 0, 0, 10 };
	
	P = (Player) { start_vector.x, start_vector.y, 90, P_stats, P_stats_mult, P_stats_total, PLAYER_HITBOX, level};
	CreateWaveTracker();
	CreateItemTracker();
	MobLoadImage();
	ItemLoadImage();
	
	
	CameraDemo_Init();
	Bulletinit();
}

void map_Update(void) {
	P.STATTOTAL.MAX_HP_TOTAL = P.STAT.MAX_HP * P.STATMULT.MAX_HP_MULT;
	P.STATTOTAL.SPEED_TOTAL = P.STAT.SPEED * P.STATMULT.SPEED_MULT;
	P.STATTOTAL.DAMAGE_TOTAL = P.STAT.DAMAGE * P.STATMULT.DAMAGE_MULT;
	P.STATTOTAL.DEFENSE_TOTAL = P.STAT.DEFENSE * P.STATMULT.DEFENSE_MULT;

	
	if (isPaused) {
		// Opens up the Upgrade Screen for players to pick their upgrades
		if (isMenu) {
			upgrade_screen(&P, &isMenu, &isPaused);
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
		
		
		if (CP_Input_KeyTriggered(KEY_ESCAPE))
			isPaused = 1;

		// Increase speed of the player
		if (CP_Input_KeyTriggered(KEY_H)) {
			P.STATMULT.SPEED_MULT *= 1.1f;
		}

		// Open up the Upgrade Screen
		if (CP_Input_KeyTriggered(KEY_U) && isMenu == 0) {
			isMenu = 1;
			isPaused = 1;
		}

		// Testing for leveling up
		if (CP_Input_KeyDown(KEY_L)) {
			P.LEVEL.P_EXP += 5;
			level_up(&P.LEVEL.P_EXP, &P.LEVEL.EXP_REQ, &P.LEVEL.VAL);
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

		// Any objects below this function will be displaced by the camera movement
		CameraDemo_Update(&P, &transform);
		
		GenerateWaves(&P);
		
		for (int w = 0; w < NO_WAVES; w++) {
			cWave = &WaveTracker[w];
			if (WaveIDQueue[w] == -1) {
				continue;
			}
			if (cWave->CurrentCount == 0) {
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
					MobTMobCollision(cMob, &P, &WaveTracker, NO_WAVES);
					MobTPlayerCollision(cMob, &P);

					int bchecker;
					if ((bchecker = BulletCollision(cMob->x, cMob->y, cMob->CStats.size)) >= 0 && bullet[bchecker].friendly == BULLET_PLAYER)
					{
						cMob->CStats.HP -= bullet[bchecker].damage;
						if (cMob->CStats.HP <= 0)
							cMob->Status = 0;
						bullet[bchecker].exist = FALSE;
					}
					if (cMob->Status == 0) {
						cWave->CurrentCount -= 1;
						MobCount[w] -= 1;
						Item *dropI = CreateItemEffect(cMob->x, cMob->y);
						ItemTracker->tree = insertItemNode(ItemTracker->tree, dropI, 0);
						ItemTracker->itemCount += 1;
						continue;
					}
					//cMob->h == 0 means haven drawn before. / assigned image to it yet
					if (P.x - WWidth/2 - cMob->w < cMob->x && cMob->x < P.x + WWidth/2 + cMob->w && P.y - WHeight/2 - cMob->h < cMob->y && cMob->y < P.y + WHeight/2 + cMob->h || cMob->h == 0) {
						DrawMobImage(cMob, &P);
					}
				}

			}
		}
		ItemPlayerCollision(&P);
		//DrawItemTree(ItemTracker->tree);
	
		if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT))
		{
			mousex = CP_Input_GetMouseWorldX();
			mousey = CP_Input_GetMouseWorldY();
			float bulletangle = 0;
			bulletangle = point_point_angle(P.x, P.y, mousex, mousey);
			BulletShoot(P.x, P.y, bulletangle, 1, BULLET_PLAYER);
		}
		BulletDraw();
		CP_Settings_ResetMatrix();

		// Time, returns and draws text
		timer(0, isPaused);
	}
	
	
	show_healthbar(&P);
	show_level(&P);

	if (CP_Input_KeyTriggered(KEY_SPACE))
		CP_Engine_Terminate();

}

void map_Exit(void) {
	FreeMobResource();
	
	FreeItemResource();
	//free(ItemTracker);
}
