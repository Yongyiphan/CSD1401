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

int wHeight, wWidth;

//Declaring player variables
Player P; Stats P_stats; StatsMult P_stats_mult; StatsTotal P_stats_total; LEVEL level;

//Sprite Stuff
#define Mob_Img 4
CP_Image** MobSprites;

CP_Vector start_vector;
CP_Color grey, black, red, green, blue, white;
CP_Matrix transform;

//Starting Quantity
int StartMobQuantity = 10, StartItemQuantity = 1000;

//Mob Stuff
#define NO_WAVES 1
#define Spawn_Timer 1
#define Wave_Timer 5
#define MaxMobGrowthRate 1
#define WaveCostGrowthRate 50
#define SpawnAreaOffset 1200

Mob* cMob;
int cWaveCost, MaxMob;
int currentSec = 0;
int WaveIDQueue[NO_WAVES];
WaveTrack WaveTracker[NO_WAVES], *cWave; // pause state for the game when paused.


//Might be useful variable for Waves Tracking
int totalWave = 0, MobCount[NO_WAVES];
float mousex, mousey;
int isPaused, isMenu, isDead;

//Images
CP_Image background = NULL;

void map_Init(void) {
	//CP_System_SetFrameRate(60);
	CP_System_SetWindowSize(MAP_SIZEX, MAP_SIZEY);
	wHeight = CP_System_GetWindowHeight() / 2;
	wWidth = CP_System_GetWindowWidth() / 2;
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
	cWaveCost = 10;
	MaxMob = 10;
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
			malloc(sizeof(Mob*) * StartMobQuantity) //, //Arr
		};
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
	Bulletinit();
}

void map_Update(void) {
	P.STATTOTAL.MAX_HP_TOTAL = P.STAT.MAX_HP * P.STATMULT.MAX_HP_MULT;
	P.STATTOTAL.SPEED_TOTAL = P.STAT.SPEED * P.STATMULT.SPEED_MULT;
	P.STATTOTAL.DAMAGE_TOTAL = P.STAT.DAMAGE * P.STATMULT.DAMAGE_MULT;
	P.STATTOTAL.DEFENSE_TOTAL = P.STAT.DEFENSE * P.STATMULT.DEFENSE_MULT;
	/*
	CP_Settings_ApplyMatrix(transform);
	CP_Settings_StrokeWeight(0.0);
	for (int x_pos = -1; x_pos < 2; x_pos++) {
		for (int y_pos = -1; y_pos < 2; y_pos++) {
			//if (P.x)
			float mapX = MAP_SIZEX / 2;
			float mapY = MAP_SIZEY / 2;

			int playerMapx = P.x / MAP_SIZEX - 650;
			int playerMapy = P.y / MAP_SIZEY - 450;
			
			/*CP_Image_Draw(background, playerMapx * mapX * x_pos, playerMapy * mapY * y_pos, MAP_SIZEX, MAP_SIZEY, 255);
			printf("playerMapx: %d\tmap coords: %f\tplayer coords:%f\t%f\n", playerMapx, playerMapx * mapX * x_pos, P.x, P.y);

			// center of the map
	CP_Image_DrawAdvanced(background, mapX + (MAP_SIZEX * x_pos), mapY + (MAP_SIZEY * y_pos), MAP_SIZEX, MAP_SIZEY, 255, 0);
}
	}*/

	/*Check the player coordinates
	create the map in a 3 x 3 dimension starting from the center
	everytime the player moves to more than half of the width or height of the image
	generate images in that direction*/
	//CP_Settings_ResetMatrix();

	
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

		if ((int)CP_System_GetSeconds() != currentSec) {
			currentSec = (int)CP_System_GetSeconds();
			printf("\n\tCurrent Sec: %d | Current FPS:%f\n", currentSec, CP_System_GetFrameRate());
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
				//PrintWaveStats(&totalWave, NO_WAVES, &WaveIDQueue, &MobCount);
			}
		}

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
					if ((bchecker = BulletCollision(cMob->x, cMob->y, cMob->w, cMob->h)) >= 0 && bullet[bchecker].friendly == BULLET_PLAYER
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
						continue;
					}
					//cMob->h == 0 means haven drawn before. / assigned image to it yet
					if (P.x - wWidth - cMob->w < cMob->x && cMob->x < P.x + wWidth + cMob->w && P.y - wHeight - cMob->h < cMob->y && cMob->y < P.y + wHeight + cMob->h || cMob->h == 0) {
						DrawMobImage(MobSprites, cMob, &P);
					}
				}

			}
		}
		//printf("MobCount: %d |\tFPS: %f \n", MobC, CP_System_GetFrameRate());
		static float bulletcd = 99; // Random big number so no cd on first shot
		static btype = 1;
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
	FreeMobResource(&WaveTracker, NO_WAVES, MobSprites, Mob_Img);
	free(MobSprites);
	
	//FreeItemResource(&ItemTracker);
	//free(ItemTracker);
}