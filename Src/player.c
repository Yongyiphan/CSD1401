#include "player.h"
#include "cprocessing.h"
#include <stdio.h>
#include "Items.h"

// Player hitbox is a circle
#define PLAYER_DIAMETER 50.0f
#define PLAYER_SPEED 20

void show_healthbar(Player p) {
	float x_coord = 50;
	float y_coord = 40;
	int rectWidth = 300;
	int rectHeight = 30;
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_Fill(CP_Color_Create(255, 200, 200, 255));
	CP_Graphics_DrawRect(x_coord, y_coord, rectWidth, rectHeight);
	CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
	CP_Graphics_DrawRect(x_coord, y_coord, p.CURRENT_HP / p.MAX_HP * rectWidth, rectHeight);
}