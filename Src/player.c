#include "cprocessing.h"
#include <stdio.h>
#include "utils.h"
#include "player.h"
#include "game.h"
#include "map.h"


// Player hitbox is a circle
#define PLAYER_DIAMETER 50.0f
#define PLAYER_SPEED 20


int collide_bullet(Player p, Bullet bullet) {
	if (CP_Math_Distance(p.x, p.y, bullet.x, bullet.y) < p.HITBOX / 2 + bullet.hitbox / 2) {
		p.CURRENT_HP -= 5;
		return 1;
	}
	return 0;
}

void shoot_bullet(Bullet bullet) {
	
}

/*
Shows healthbar of the player. Creates 2 rectangles, one specifying current HP, and the other max HP.
Current HP is always proportional to the length of max HP.
*/
void show_healthbar(Player *p) {
	float x_coord = 100;
	float y_coord = 40;
	int rectWidth = 300;
	int rectHeight = 30;
	/*CP_Settings_TextSize(40.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
	CP_Font_DrawText("Health", 10, 10);*/
	CP_Settings_Fill(CP_Color_Create(255, 200, 200, 255));
	CP_Settings_StrokeWeight(0);
	CP_Graphics_DrawRectAdvanced(x_coord, y_coord, rectWidth, rectHeight, 0, 10);
	CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));

	if (p->CURRENT_HP > p->MAX_HP)
		p->CURRENT_HP = p->MAX_HP;
	else if (p->CURRENT_HP <= 0)
		p->CURRENT_HP = 0;

	CP_Graphics_DrawRectAdvanced(x_coord, y_coord, p->CURRENT_HP / p->MAX_HP * rectWidth, rectHeight, 0, 10);
}


// Shows a death screen, and gives the player the option whether to restart the game
// or exit to main menu.
void death_screen(int *isPaused) {
	*isPaused = 1;
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
	CP_Font_DrawText("Restart Game?", middle.x, middle.y);
	CP_Font_DrawText("Exit to main menu", middle.x, middle.y + height + padding);


	// Remove rectangle align-center and add stroke back in
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));


	if (CP_Input_MouseClicked()) {
		if (IsAreaClicked(middle.x, middle.y, width, height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			// Not working!!!!!
			// CP_Engine_SetNextGameState(map_Init, map_Init, map_Exit);
		}
		if (IsAreaClicked(middle.x, middle.y + height + padding, width, height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Engine_SetNextGameState(game_Init, game_Update, game_Exit);
		}
	}
}

// Show player's available upgrades
void upgrade_screen(Player* P, int* isMenu, int* isPaused) {
	
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

	int countStats = 0;
	//float stats[] = { P->CURRENT_HP, P->MAX_HP, P->DAMAGE, P->SPEED, P->DEFENSE, P->ATK_SPEED };
	char* name[] = { "Bullet Speed", "Max HP", "Damage", "Speed", "Defense", "Attack Speed" };

	CP_Settings_RectMode(CP_POSITION_CENTER);

	CP_Settings_NoStroke();
	/*CP_Settings_Stroke((CP_Color_Create(0, 255, 0, 255)));
	CP_Settings_StrokeWeight(2.0f);*/

	// Options background
	CP_Settings_Fill(CP_Color_Create(80, 80, 100, 120));
	CP_Graphics_DrawRect(middle.x, middle.y, background_width, background_height);

	// Text Adjustments
	CP_TEXT_ALIGN_HORIZONTAL centerHor = CP_TEXT_ALIGN_H_CENTER;
	CP_TEXT_ALIGN_VERTICAL centerVert = CP_TEXT_ALIGN_V_MIDDLE;
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));		// text is black
	CP_Settings_TextSize(30.0f);
	CP_Settings_TextAlignment(centerHor, centerVert);

	// Draw out option boxes with padding applied
	float background_topX = middle.x - background_width / 2;
	float background_topY = middle.y - background_height / 2;
	float currentX = background_topX, currentY = background_topY;
	for (int i = 0; i < numBoxes_vert; i++) {
		for (int y = 0; y < numBoxes_hor; y++) {
			float boxX = currentX + padding + box_width / 2;
			float boxY = currentY + padding + box_height / 2;
			CP_Settings_Fill(CP_Color_Create(255, 100, 100, 255));
			CP_Graphics_DrawRect(boxX, boxY, box_width, box_height);
			CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
			CP_Font_DrawText(name[countStats % 6], boxX, boxY);

			if (CP_Input_MouseClicked()) {
				if (IsAreaClicked(boxX, boxY, box_width, box_height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
					//stats[countStats % 6] += 1;
					if (*name[countStats % 6] == "Max HP")
						P->MAX_HP += 3;
					else if (*name[countStats % 6] == "Damage")
						P->DAMAGE += 1;
					else if (*name[countStats % 6] == "Speed")
						P->SPEED += 2;
					else if (*name[countStats % 6] == "Defense")
						P->DEFENSE += 1;
					//printf(":%s\t%f\n", name[countStats % 6], stats[countStats % 6]);
				}
			}
				

			currentX += box_width + padding;
			countStats++;
		}
		currentX = background_topX, currentY = background_topY;
		currentY += box_height + padding;
	}
	if (CP_Input_KeyTriggered(KEY_I)) {
		*isPaused = 0;
		*isMenu = 0;
	}

	// Remove rectangle align-center and add stroke back in
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
}