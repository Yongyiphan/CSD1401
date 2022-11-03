#pragma once


#include "player.h"

#define Empty -1
#define EXP 0
#define StatBoost 1 //random stat boost



typedef struct ItemStat {
	char* StatusEffect; //char arr -> string :)
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

typedef struct ItemNode {
	Item key;
	ItemNode* left;
	ItemNode* right;
	int h; //height of current node;
}ItemNode;

typedef struct ItemTracker {
	Item** arr;
	int arrSize;
	int itemCount;
	ItemNode* tree;
}ItemTrack;




int TreeHeight(ItemNode* current);
int getBalance(ItemNode* current);

ItemNode* newNode(Item i);
ItemNode* insert(ItemNode* root, Item i);
ItemNode* rightRotate(ItemNode* i);
ItemNode* leftRotate(ItemNode* i);

void freeTree(ItemNode* root);
float getX(ItemNode* current);



/*
Item Planning

Player will not know what the item's effect:


Player Stat Boost -> Limited Duration
Exp -> Dropped by Mobs (Mitigation -> 
Magnet -> Moves Exp to Player's location
Bullet Augm

Item Storage
1 Continuous Array of ? items
Mobs have a chance to drop items upon death
*/
void InitItemArr(ItemTrack* tracker);

void CreateItemEffect(Item* i);
void IAffectPlayer(Item* i, Player* p, int currentSec);

void FreeItemResource(ItemTrack* tracker);




