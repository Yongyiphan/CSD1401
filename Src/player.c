#include "player.h"
#include "cprocessing.h"
#include <stdio.h>

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

/*int collide_bullet(Player p, Bullet bullet) {
	if (CP_Math_Distance(p.x, p.y, bullet.x, bullet.y) < p.HITBOX / 2 + bullet.hitbox / 2) {
		p.CURRENT_HP -= 5;
		return 1;
	}
	return 0;
}

void shoot_bullet(Bullet bullet) {
	
} */

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