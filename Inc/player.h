#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "bullet.h"
#include "cprocessing.h"

extern int NoBaseStats;

typedef struct PlayerStats {
	float MAX_HP;
	float SPEED;
	float DAMAGE;
	float ATK_SPEED;
	float DEFENSE;
	float PICKUP;
}Stats;

typedef struct PlayerStatsMultiplier {
	float MAX_HP_MULT;
	float SPEED_MULT;
	float DAMAGE_MULT;
	float ATK_SPEED_MULT;
	float DEFENSE_MULT;
	float PICKUP_MULT;
}StatsMult;

typedef struct PlayerStatsTotal {
	float MAX_HP_TOTAL;
	float SPEED_TOTAL;
	float DAMAGE_TOTAL;
	float ATK_SPEED_TOTAL;
	float DEFENSE_TOTAL;
	float PICKUP_TOTAL;
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
	CP_Vector coor;
	
}Player;

#define PLAYER_HP 300.0f
#define PLAYER_SPEED 300.0f
#define PLAYER_DAMAGE 1.0f
#define ATK_SPD 2.0f
#define PLAYER_DEFENSE 10
#define PLAYER_HITBOX 50
#define PLAYER_PICKUP 50




// Initializes player stats
void Player_Init(Player* player);

void Player_Stats_Update(Player* player);

void Player_Show_Stats(Player player);

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

char* GetBaseStats(int type);


#endif