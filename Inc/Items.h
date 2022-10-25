#pragma once
#ifndef ITEMS_H
#define ITEMS_H
#include "player.h"

typedef struct ItemStat {
	int Hitbox;



}Item;



void CreateItemEffect(Item* i);
void IAffectPlayer(Item* i, Player* p);

#endif
