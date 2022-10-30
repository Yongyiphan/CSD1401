#pragma once

typedef struct Player{
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

typedef struct Items {
	float hitbox;
	float x;
	float y;

}Items;

typedef struct Bullet {
	float hitbox;
	float x;
	float y;

}Bullet;

//int collide_mob(Player, Mob);
int collide_item(Player, Items);
int collide_bullet(Player, Bullet);

void shoot_bullet(Bullet);

/*
Shows healthbar of the player. Creates 2 rectangles, one specifying current HP, and the other max HP.
Current HP is always proportional to the length of max HP.
*/
void show_healthbar(Player*);

// Create a death screen when player is dead.
void death_screen(int*);

// Show the upgrade screen for players to upgrade their stats.
void upgrade_screen(Player*, int*, int*);