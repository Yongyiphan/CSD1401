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

//typedef struct Mob {
//	float hitbox;
//	float x;
//	float y;
//}Mob;


typedef struct Bullet {
	float hitbox;
	float x;
	float y;

}Bullet;

//int collide_mob(Player, Mob);
int collide_bullet(Player, Bullet);

void shoot_bullet(Bullet);

void show_healthbar(Player);