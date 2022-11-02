#pragma once
#ifndef PLAYER_H
#define PLAYER_H



typedef struct PlayerStats {
	float MAX_HP;
	float SPEED;
	float DAMAGE;
	float ATK_SPEED;
	float DEFENSE;
}Stats;

typedef struct PlayerStatsMultiplier {
	float MAX_HP_MULT;
	float SPEED_MULT;
	float DAMAGE_MULT;
	float ATK_SPEED_MULT;
	float DEFENSE_MULT;
}StatsMult;

typedef struct PlayerStatsTotal {
	float MAX_HP_TOTAL;
	float SPEED_TOTAL;
	float DAMAGE_TOTAL;
	float ATK_SPEED_TOTAL;
	float DEFENSE_TOTAL;
}StatsTotal;

typedef struct LEVEL {
	int VAL;
	int P_EXP;
	int EXP_REQ;
}LEVEL;

typedef struct Player{
	float x;
	float y;
	float CURRENT_HP;
	Stats STAT;
	StatsMult STATMULT;
	StatsTotal STATTOTAL;
	int HITBOX;
	LEVEL LEVEL;
	
}Player;

typedef struct Bullet {
	float hitbox;
	float x;
	float y;

}Bullet;

//int collide_mob(Player, Mob);
int collide_bullet(Player, Bullet);

void shoot_bullet(Bullet);

/*
Shows healthbar of the player. Creates 2 rectangles, one specifying current HP, and the other max HP.
Current HP is always proportional to the length of max HP.
*/
void show_healthbar(Player* player);

void show_level(Player* player);

// Create a death screen when player is dead.
void death_screen(float time);

// Show the upgrade screen for players to upgrade their stats.
void upgrade_screen(Player* player, int* isMenu, int* isPaused);


/*
Check the conditions for leveling up
When "exp" reaches the value of "exp_req":
	"exp" will reset
	"level" will increase by 1
	"exp_req" will increase, making it harder to level up
*/
void level_up(int *exp, int *exp_req, int *level);



#endif