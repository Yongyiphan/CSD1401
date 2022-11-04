#pragma once
#ifndef ITEMS_H
#define ITEMS_H


#include "player.h"

#define Empty -1
#define EXP 0
#define StatBoost 1 //random stat boost




typedef struct ItemStat {
	int AffectedBaseStat;
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

static const Item EmptyItem;

typedef struct ItemTreeNode {
	Item key;
	struct ItemNode* left;
	struct ItemNode* right;
	int h; //height of current node;
}ItemNode;

typedef struct ItemTracker {
	Item** arr;
	int arrSize;
	int itemCount;
	ItemNode* tree;
}ItemTrack;



#define StartItemQuantity 1000
extern ItemTrack* ItemTracker;



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
void CreateItemTracker(void);
void InitItemArr(ItemTrack* tracker);

Item* CreateItemEffect(float x, float y);
void IAffectPlayer(Item* i, Player* p, int currentSec);

void GenerateItem(ItemTrack* tracker, int currentSec);

void FreeItemResource(ItemTrack* tracker);

ItemNode* DrawItemTree(ItemNode* node);

int TreeHeight(ItemNode* current);
int getBalance(ItemNode* current);

ItemNode* newNode(Item item);
ItemNode* insertItemNode(ItemNode* root, Item item);
ItemNode* deleteItemNode(ItemNode* root, Item item);
ItemNode* minValueNode(ItemNode* node);
ItemNode* rightRotate(ItemNode* item);
ItemNode* leftRotate(ItemNode* item);


void freeTree(ItemNode* root);
float getX(ItemNode* current);



#endif
