#pragma once
#ifndef ITEMS_H
#define ITEMS_H


#include "player.h"

#define ItemDecayTimer 10
#define Empty -1

#define EXP 0
#define StatBoost 1




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
	//-1 = Collected, 1 = Not collected;
	//0 = initialised value
	int collected;
}Item;


static const Item EmptyItem;

//typedef struct ItemTreeNode {
//	Item *key;
//	CP_Vector point;
//	struct ItemNode* left, *right;
//	struct ItemNode* prev;	//int h; //height of current node;
//}ItemNode;


typedef struct ItemLink {
	Item* key;
	struct ItemLink* next;
}ItemLink;

typedef struct ItemTracker {
	//Item** arr;
	//int arrSize;
	int expDrops;
	ItemLink* ExpLL, *ItemLL;
	int ItemCount;
}ItemTrack;



#define StartItemQuantity 1000
extern ItemTrack* ItemTracker;

extern int Img_C;
extern CP_Image** ItemSprites;

extern int NoDeleted;
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

void ItemLoadImage(void);
void DrawItemImage(Item* item);

void CreateItemTracker(void);

Item* CreateItemEffect(float x, float y, int exp, int expVal);
void IAffectPlayer(Item* i, int method);


//void DrawItemTree(ItemNode* node);
//void PrintTree(ItemNode* root, int space, int depth);

void ItemPlayerCollision(void);

void copyItem(Item* dst, Item* src);


ItemLink* DrawItemLink(ItemLink* head);
ItemLink* newLink(Item* item);
void insertItemLink(ItemLink** head, Item *item);
void deleteItemLink(ItemLink** head, Item *item);
void freeLink(ItemLink* head);

extern int failedDelete;

//KD -Tree
//#define Dimension 2
//ItemNode* newNode(Item *item);
//extern int insertSuccess;
//ItemNode* insertItemNode(ItemNode* root, Item *item);
//ItemNode* insertItemRec(ItemNode* prev, ItemNode* root, Item *item, unsigned depth);
//ItemNode* minNode(ItemNode* root, ItemNode* left, ItemNode* right, int d);
//ItemNode* findMin(ItemNode* root, int d, unsigned int depth);
//ItemNode* deleteItemNode(ItemNode* root, CP_Vector point, unsigned int depth);
//
////returns the searched item node
//ItemNode* nearestNeighbour(ItemNode* root, CP_Vector point, unsigned int depth);
//ItemNode* closest(ItemNode* n0, ItemNode* n1, CP_Vector point);
//
//void CleanTree(ItemNode* root);
//void freeTree(ItemNode* root);

void FreeItemResource(void);






/*
#pragma region
int TreeHeight(ItemNode* current);
int getBalance(ItemNode* current);

ItemNode* newNode(Item item);
ItemNode* insertItemNode(ItemNode* root, Item item);
ItemNode* deleteItemNode(ItemNode* root, Item item);
ItemNode* minValueNode(ItemNode* node);
ItemNode* rightRotate(ItemNode* item);
ItemNode* leftRotate(ItemNode* item);

float getX(ItemNode* current);
void freeTree(ItemNode* root);
#pragma endregion
*/

#endif
