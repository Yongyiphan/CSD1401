#pragma once
#ifndef ITEMS_H
#define ITEMS_H


#include "player.h"

#define ItemDecayTimer 10
#define Empty -1

#define No_Items 4
#define EXP 0
#define StatBoost 1
#define MAGNET 2
#define COIN 3




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
	float Modifier; //Stat Modified amt

	//Coordinates
	//double x;
	//double y;
	CP_Vector coor;
	//-1 = Collected, 1 = Not collected, 2 = applying;
	//0 = initialised value
	int collected, applying;
	int knockback;
	int Dis[2];
}Item;


static const Item EmptyItem;



typedef struct ItemLink {
	Item* key;
	struct ItemLink* next;
}ItemLink;

typedef struct ItemTracker {
	//Item** arr;
	//int arrSize;
	ItemLink* ExpLL, *ItemLL, *CoinLL;
	int DropCount[No_Items][2];
}ItemTrack;




#define StartItemQuantity 1000
extern ItemTrack* ItemTracker;

extern int Img_C;
extern CP_Image** ItemSprites;

/*
Item Planning

Player will not know what the item's effect:


Player Stat Boost -> Limited Duration
Exp -> Dropped by Mobs (Mitigation -> 
Magnet -> Moves Exp to Player's location
Bullet Augm

Item Storage
1 Big Tree	-> insert when mob die (exp) | items 
			-> collision check player's position with items
Mobs have a chance to drop items upon death
*/
ItemLink* GetCurrentEffects(void);
void UpdateAppliedEffects(Item* item);
void DrawAppliedEffects(void);

void ItemLoadImage(void);
void DrawItemImage(Item* item);

void CreateItemTracker(void);
int ItemCountSum(void);
Item* CreateItemEffect(CP_Vector coor, int exp, int expVal);
void IAffectPlayer(Item* i, int method);




ItemLink* ItemInteraction(ItemLink* head);
ItemLink* newLink(Item* item);
void insertItemLink(ItemLink** head, Item *item);
void deleteItemLink(ItemLink** head, Item *item);
void freeLink(ItemLink* head);



void FreeItemResource(void);




#endif
