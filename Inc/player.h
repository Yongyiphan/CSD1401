#pragma once

typedef struct Player {
	float x;
	float y;
	float CURRENT_HP;
	float MAX_HP;
	float SPEED;
	float DAMAGE;
	float ATK_SPEED;
	float DEFENSE;
	int HITBOX;
}Player;

void show_healthbar(Player);