#pragma once
#ifndef ITEMS_H
#define ITEMS_H
#include "player.h"

#define Empty -1
#define EXP 0
#define StatBoost 1 //random stat boost



typedef struct ItemStat {
	int Hitbox;
	int Start;
	/*
	While currentSec (map.c) - start < duration
		Apply Effect
	*/
	int Duration;// In Secs //-1 == Permanent
	int Type;//Effect Type
	int Modifier; //Stat Modified amt

	//Coordinates
	double x;
	double y;
}Item;

typedef struct ItemTracker {
	Item* arr;
	int arrSize;
	int itemCount;
}ItemTracker;


/*
Item Planning
Player Stat Boost -> Limited Duration
Exp -> Dropped by Mobs
Magnet -> Moves Exp to Player's location
Bullet Augm
*/


void CreateItemEffect(Item* i);
void IAffectPlayer(Item* i, Player* p, int currentSec);

#endif
