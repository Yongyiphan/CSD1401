#pragma once
#include "cprocessing.h"
#include <stdio.h>
#include "map.h"
#include "player.h"
#include "game.h"
#include "utils.h"
#include "Mob.h"
#include "Items.h"
#include "mainmenu.h"
#include "upgrades.h"


// Player hitbox is a circle

#define _countof(array) (sizeof(array) / sizeof(array[0]))

//Declaring player variables
CP_Vector start_vector;
Stats P_stats; StatsMult P_stats_mult; StatsTotal P_stats_total; LEVEL level;

Player P;
void Player_Init(Player* P) {
	/*
	P_stats: Base stats of the player, can only be altered outside of the game.
	P_stats_mult: In-game stats upgrade. Player stats are increased by multiplying its value against the player base stats
	P_stats_total: The total amount of stats by multiplying its value with player base stats.
	E.G. P_stats's MAX_HP = 100, P_stats_mult's MAX_HP_MULT = 1.2
	P_stats_total's MAX_HP_TOTAL = MAX_HP * MAX_HP_MULT
								 = 100 * 1.2 = 120
	*/

	start_vector = CP_Vector_Zero();
	P_stats = (Stats){ PLAYER_HP + upgrades[0].stat, PLAYER_SPEED + upgrades[1].stat, PLAYER_DAMAGE + upgrades[2].stat, ATK_SPD + upgrades[3].stat, PLAYER_DEFENSE + upgrades[4].stat , PLAYER_PICKUP + upgrades[5].stat, PLAYER_PROJ_SPD + upgrades[6].stat};
	P_stats_mult = (StatsMult){ 1, 1, 1, 1, 1, 1, 1 };
	P_stats_total = (StatsTotal){ PLAYER_HP, PLAYER_SPEED, PLAYER_DAMAGE, ATK_SPD, PLAYER_DEFENSE, PLAYER_PICKUP, PLAYER_PROJ_SPD};
	level = (LEVEL){ 0, 0, 50 };

	*P = (Player){ start_vector.x, start_vector.y, PLAYER_HP + upgrades[0].stat, P_stats, P_stats_mult, P_stats_total, PLAYER_HITBOX, level};
	P->coor = CP_Vector_Set(P->x, P->y);
}


// Update Player's Total Stats where the base HP is multiplied by the respective stat multipliers.
void Player_Stats_Update(Player* P) {
	P->STATTOTAL.MAX_HP_TOTAL = P->STAT.MAX_HP * P->STATMULT.MAX_HP_MULT;
	P->STATTOTAL.SPEED_TOTAL = P->STAT.SPEED * P->STATMULT.SPEED_MULT;
	P->STATTOTAL.DAMAGE_TOTAL = P->STAT.DAMAGE * P->STATMULT.DAMAGE_MULT;
	P->STATTOTAL.ATK_SPEED_TOTAL = P->STAT.ATK_SPEED * P->STATMULT.ATK_SPEED_MULT;
	P->STATTOTAL.PICKUP_TOTAL = P->STAT.PICKUP * P->STATMULT.PICKUP_MULT;
	P->STATTOTAL.PROJECTILE_SPD_TOTAL = P->STAT.PROJECTILE_SPD * P->STATMULT.PROJECTILE_SPD_MULT;
	
}

/*
Shows Player's stats: HP, SPEED, DMG, ATK_SPEED, PICKUP_RADIUS and BULLET_SPD.
*/
void Player_Show_Stats(Player P) {
	float printX = CP_System_GetWindowWidth() * 0.7 / 10;
	float printY = CP_System_GetWindowHeight() * 7.5 / 10;
	float padding = 30;
	char bufferList[6][30] = { {0},{0},{0},{0},{0},{0} };
	char* bufferName[] = { "HP: ", "SPEED: ", "DAMAGE: ", "FIRE RATE: ", "PICKUP RADIUS: ", "BULLET SPEED: " };


	CP_Settings_TextSize(30.0f);
	CP_Settings_Fill(CP_Color_Create(244, 244, 244, 255));
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
	sprintf_s(bufferList[0], _countof(bufferList[0]), "%.f", P.STATTOTAL.MAX_HP_TOTAL);
	sprintf_s(bufferList[1], _countof(bufferList[1]), "%.f", P.STATTOTAL.SPEED_TOTAL);
	sprintf_s(bufferList[2], _countof(bufferList[2]), "%.f", P.STATTOTAL.DAMAGE_TOTAL);
	sprintf_s(bufferList[3], _countof(bufferList[3]), "%.2f", P.STATTOTAL.ATK_SPEED_TOTAL);
	sprintf_s(bufferList[4], _countof(bufferList[4]), "%.f", P.STATTOTAL.PICKUP_TOTAL);
	sprintf_s(bufferList[5], _countof(bufferList[5]), "%.2f", P.STATTOTAL.PROJECTILE_SPD_TOTAL);

	for (int i = 1; i < 6; i++) {
		CP_Font_DrawText(bufferName[i], printX, printY + padding * i);
		CP_Font_DrawText(bufferList[i], printX + 200, printY + padding * i);
	}
	/*sprintf_s(buffer, _countof(buffer), "%d", P.STATTOTAL.MAX_HP_TOTAL);
	CP_Font_DrawText(buffer,)*/
}


void Player_Show_Coins(void) {
	float printX = CP_System_GetWindowWidth() * 7.0 / 10;
	float printY = CP_System_GetWindowHeight() * 1.0 / 10;

	
	CP_Image_Draw(ItemSprites[COIN], printX + 200, printY + 17, 50, 50, 255);
	char buffer[16] = { 0 };
	sprintf_s(buffer, _countof(buffer), "%d", P.STAT.Coin_Gained);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
	CP_Settings_TextSize(30.0f);
	CP_Font_DrawText("COINS GAINED:", printX, printY);
	CP_Settings_TextSize(60.0f);
	CP_Font_DrawText(buffer, printX - 20, printY + 30);
	CP_Settings_TextSize(30.0f);
}

/*
Shows healthbar of the player. Creates 2 rectangles, one specifying current HP, and the other max HP.
Current HP is always proportional to the length of max HP.
*/
void show_healthbar(Player* p) {
	CP_Settings_RectMode(CP_POSITION_CORNER);
	float x_coord = (float)CP_System_GetWindowWidth() * 1 / 10;
	float y_coord = (float)CP_System_GetWindowHeight() * 0.65 / 10;
	int rectWidth = 300;
	int rectHeight = 30;

	CP_Settings_Fill(CP_Color_Create(255, 200, 200, 255));
	CP_Settings_StrokeWeight(3.0f);
	//CP_Settings_TextSize(20.0f);
	CP_Graphics_DrawRectAdvanced(x_coord, y_coord, rectWidth, rectHeight, 0, 0);
	CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));

	if (p->CURRENT_HP > p->STAT.MAX_HP)
		p->CURRENT_HP = p->STAT.MAX_HP;
	else if (p->CURRENT_HP <= 0)
		p->CURRENT_HP = 0;

	CP_Graphics_DrawRectAdvanced(x_coord, y_coord, p->CURRENT_HP / p->STAT.MAX_HP * rectWidth, rectHeight, 0, 0);
	CP_Settings_RectMode(CP_POSITION_CENTER);
	
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_TOP);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	char buffer[2][16] = { {0}, {0} };
	sprintf_s(buffer[0], _countof(buffer[0]), "%.f", P.CURRENT_HP);
	sprintf_s(buffer[1], _countof(buffer[1]), "%.f", P.STATTOTAL.MAX_HP_TOTAL);
	CP_Font_DrawText(buffer[0], (x_coord * 2 + rectWidth) / 2 - 40, y_coord);
	CP_Font_DrawText("/", (x_coord * 2 + rectWidth) / 2, y_coord);
	CP_Font_DrawText(buffer[1], (x_coord * 2 + rectWidth) / 2 + 40, y_coord);
}

void show_level(Player* P) {
	CP_Settings_RectMode(CP_POSITION_CORNER);
	float x_coord = (float)CP_System_GetWindowWidth() * 1 / 10;
	float y_coord = (float)CP_System_GetWindowHeight() * 1.1 / 10;

	int rectWidth = 300;
	int rectHeight = 30;

	/*CP_Settings_TextSize(40.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
	CP_Font_DrawText("Health", 10, 10);*/
	CP_Settings_Fill(CP_Color_Create(200, 200, 255, 255));
	CP_Settings_StrokeWeight(3.0f);
	CP_Graphics_DrawRectAdvanced(x_coord, y_coord, rectWidth, rectHeight, 0, 0);

	CP_Settings_Fill(CP_Color_Create(100, 200, 100, 255));
	CP_Graphics_DrawRectAdvanced(x_coord, y_coord, (P->LEVEL.P_EXP / (float) P->LEVEL.EXP_REQ) * rectWidth, rectHeight, 0, 0);
	CP_Settings_RectMode(CP_POSITION_CENTER);

	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_TOP);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	char buffer[3][16] = { {0}, {0}, {0} };
	sprintf_s(buffer[0], _countof(buffer[0]), "%d", P->LEVEL.VAL);
	sprintf_s(buffer[1], _countof(buffer[1]), "%d", P->LEVEL.P_EXP);
	sprintf_s(buffer[2], _countof(buffer[2]), "%d", P->LEVEL.EXP_REQ);
	CP_Font_DrawText("Lv:", (x_coord * 2 + rectWidth) / 2 - 110, y_coord);
	CP_Font_DrawText(buffer[0], (x_coord * 2 + rectWidth) / 2 - 85, y_coord);
	CP_Font_DrawText(buffer[1], (x_coord * 2 + rectWidth) / 2 - 40, y_coord);
	CP_Font_DrawText("/", (x_coord * 2 + rectWidth) / 2, y_coord);
	CP_Font_DrawText(buffer[2], (x_coord * 2 + rectWidth) / 2 + 40, y_coord);
}

// Shows a death screen, and gives the player the option whether to restart the game
// or exit to main menu.
void death_screen(float totalElapsedTime) {

	float screen_width = (float) CP_System_GetWindowWidth();
	float screen_height = (float) CP_System_GetWindowHeight();
	CP_Vector middle = CP_Vector_Set(screen_width / 2, screen_height / 2);

	float width = 300;
	float height = 60;
	float padding = 30;

	float textSize = 30.0;

	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_NoStroke();
	// Options background
	CP_Settings_Fill(CP_Color_Create(80, 80, 100, 120));
	CP_Graphics_DrawRect(middle.x, middle.y, screen_width * 4.0 / 10, screen_height * 8.0 / 10);

	// Draw out option boxes
	CP_Settings_Fill(CP_Color_Create(255, 100, 100, 255));
	CP_Graphics_DrawRect(middle.x, middle.y, width, height);
	CP_Graphics_DrawRect(middle.x, middle.y + height + padding, width, height);

	// Align text to the center vertically and horizontally
	CP_TEXT_ALIGN_HORIZONTAL centerHor = CP_TEXT_ALIGN_H_CENTER;
	CP_TEXT_ALIGN_VERTICAL centerVert = CP_TEXT_ALIGN_V_MIDDLE;
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));		// text is black

	// Draw text of respective boxes at respective coordinates.
	CP_Settings_TextSize(40.0f);
	CP_Settings_TextAlignment(centerHor, centerVert);

	char buffer[30] = { 0 };
	// Print out the words shown
	CP_Font_DrawText("You have lasted for ", middle.x - 50, screen_height * 4/10);
	
	// Show how long the player survived for
	sprintf_s(buffer, _countof(buffer), "%.2f", totalElapsedTime);
	CP_Font_DrawText(buffer, middle.x + 140, screen_height * 4 / 10);

	// The buttons to restart and exit game
	CP_Font_DrawText("Restart Game?", middle.x, middle.y);
	CP_Font_DrawText("Exit to main menu", middle.x, middle.y + height + padding);


	// Remove rectangle align-center and add stroke back in
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));


	if (CP_Input_MouseClicked()) {
		if (IsAreaClicked(middle.x, middle.y, width, height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			FreeMobResource();
			FreeItemResource();
			map_Init();
		}
		if (IsAreaClicked(middle.x, middle.y + height + padding, width, height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		}
	}
}

#define Level_Req_Jump 10
#define Level_Jump_Perc 1.5f
#define Level_Up_Perc 1.2f
int level_up(LEVEL* level) {
	if (level->P_EXP >= level->EXP_REQ) {
		level->P_EXP = 0;
		level->EXP_REQ = level->VAL % Level_Req_Jump == 0 ? level->EXP_REQ * Level_Jump_Perc : level->EXP_REQ * Level_Up_Perc;
		level->VAL += 1;

		return 1;
	}
	return 0;
}

// Show player's available upgrades
void upgrade_screen(Player* P, int* isUpgrade, int* isPaused) {
	
	float screen_width = (float) CP_System_GetWindowWidth();
	float screen_height = (float) CP_System_GetWindowHeight();
	CP_Vector middle = CP_Vector_Set(screen_width / 2, screen_height / 2);

	float background_width = screen_width * 6.0 / 10;
	float background_height = screen_height * 7.0 / 10;
	float upgrades_height = screen_height * 4.0 / 10;

	float padding = 40;
	int numBoxes_hor = 3, numBoxes_vert = 2;
	// Apply the text boxes' width and height according to the number of boxes, padding,
	// and background width and height.
	const float box_width = ((background_width - (numBoxes_hor + 1) * padding)) / (float) numBoxes_hor;
	const float box_height = ((upgrades_height - (numBoxes_vert + 1) * padding)) / (float) numBoxes_vert;

	float textSize = 30.0;

	
	//float stats[] = { P->CURRENT_HP, P->MAX_HP, P->DAMAGE, P->SPEED, P->DEFENSE, P->ATK_SPEED };
	

	CP_Settings_RectMode(CP_POSITION_CENTER);

	CP_Settings_NoStroke();

	// Options background
	CP_Settings_Fill(CP_Color_Create(80, 80, 100, 120));
	CP_Graphics_DrawRect(middle.x, middle.y, background_width, background_height);

	// Text Adjustments
	CP_TEXT_ALIGN_HORIZONTAL centerHor = CP_TEXT_ALIGN_H_CENTER;
	CP_TEXT_ALIGN_VERTICAL centerVert = CP_TEXT_ALIGN_V_MIDDLE;
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));		// text is black
	
	CP_Settings_TextAlignment(centerHor, centerVert);

	char* name[] = { "Pickup Radius", "Max HP", "Damage", "Speed", "Bullet Speed", "Attack Speed" };
	int countStats = 0;
	// Draw out option boxes with padding applied
	float background_topX = middle.x - background_width / 2;
	float background_topY = middle.y - background_height / 2;
	float currentX = background_topX, currentY = background_topY + 300;
	CP_Settings_TextSize(55.0f);
	CP_Font_DrawText("You have leveled up!!", middle.x, background_topY + 70);
	CP_Font_DrawText("UPGRADES", middle.x, background_topY + 130);
	
	CP_Settings_TextSize(35.0f);
	for (int i = 0; i < numBoxes_vert; i++) {
		for (int y = 0; y < numBoxes_hor; y++) {
			float boxX = currentX + padding + box_width / 2;
			float boxY = currentY + padding + box_height / 2;
			CP_Settings_Fill(CP_Color_Create(255, 100, 100, 255));
			CP_Graphics_DrawRectAdvanced(boxX, boxY, box_width, box_height, FALSE, 10);
			CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
			CP_Font_DrawText(name[countStats % 6], boxX, boxY-10);

			if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
				if (IsAreaClicked(boxX, boxY, box_width, box_height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
					//stats[countStats % 6] += 1;
					
					if (strcmp(name[countStats % 6], "Pickup Radius") == 0) {
						P->STATMULT.PICKUP_MULT += 0.2;
						//printf("%s: %.2f\n", name[countStats % 6], P->STATMULT.MAX_HP_MULT);
					}
					else if (strcmp(name[countStats % 6], "Max HP") == 0) {
						P->STATMULT.MAX_HP_MULT += 1.0 / 100;
					}
					else if (strcmp(name[countStats % 6], "Damage") == 0) {
						P->STATMULT.DAMAGE_MULT += 0.3;
						//printf("%s: %.2f\n", name[countStats % 6], P->STATMULT.DAMAGE_MULT);
					}
					else if (strcmp(name[countStats % 6], "Speed") == 0)
						P->STATMULT.SPEED_MULT += 0.2;
					else if (strcmp(name[countStats % 6], "Bullet Speed") == 0)
						P->STATMULT.PROJECTILE_SPD_MULT += 0.15;
					else if (strcmp(name[countStats % 6], "Attack Speed") == 0)
						P->STATMULT.ATK_SPEED_MULT += 0.05;

					*isPaused = 0;
					*isUpgrade = 0;
				}
			}
				
			
			currentX += box_width + padding;
			countStats++;
		}
		currentX = background_topX, currentY = background_topY;
		currentY += box_height + padding;
	}
	if (CP_Input_KeyTriggered(KEY_U)) {
		*isPaused = 0;
		*isUpgrade = 0;
	}

	// Remove rectangle align-center and add stroke back in
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
}

char* PStats[] = {"HEALTH", "SPEED", "DAMAGE", "FIRE RATE", "BULLET SPEED", "MAX HP"};
int NoBaseStats = 6;
char* GetBaseStats(int type) {
	type = type == -1 ? -1 : type % NoBaseStats;
	switch (type) {
	case -1:
		return PStats;
	default:
		return PStats[type];
	}
}

