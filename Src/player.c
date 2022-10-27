#include "cprocessing.h"
#include <stdio.h>
#include "utils.h"
#include "player.h"
#include "game.h"
#include "map.h"


// Player hitbox is a circle
#define PLAYER_DIAMETER 50.0f
#define PLAYER_SPEED 20



//int collide_mob(Player p, Mob mob) {
//	if (CP_Math_Distance(p.x, p.y, mob.x, mob.y) < p.HITBOX / 2 + mob.hitbox / 2) {
//		p.CURRENT_HP -= 5;
//		return 1;
//	}
//	return 0;
//}

int collide_item(Player p, Items item) {
	if (CP_Math_Distance(p.x, p.y, item.x, item.y) < p.HITBOX / 2 + item.hitbox / 2) {
		// change the player stats or affect the bullet type
		return 1;
	}
	return 0;
}

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
	float x_coord = 50;
	float y_coord = 40;
	int rectWidth = 300;
	int rectHeight = 30;
	CP_Settings_Fill(CP_Color_Create(255, 200, 200, 255));
	CP_Graphics_DrawRect(x_coord, y_coord, rectWidth, rectHeight);
	CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));

	if (p->CURRENT_HP > p->MAX_HP)
		p->CURRENT_HP = p->MAX_HP;
	else if (p->CURRENT_HP <= 0)
		p->CURRENT_HP = 0;

	CP_Graphics_DrawRect(x_coord, y_coord, p->CURRENT_HP / p->MAX_HP * rectWidth, rectHeight);
}



void death_screen(void) {
	float screen_width = CP_System_GetDisplayWidth();
	float screen_height = CP_System_GetDisplayHeight();
	CP_Vector middle = CP_Vector_Set(screen_width / 2, screen_height / 2);

	int width = 300;
	int height = 60;
	int padding = 30;

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