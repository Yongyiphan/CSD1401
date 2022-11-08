#pragma once
#include "cprocessing.h"
#include <stdlib.h>
#include "Items.h"
#include "Mob.h"
#include "player.h"
#include "Map.h"
/*
@brief		Function that initialise array with empty items
@params		tracker	-> Contains stats for tracking Items
@returns	Item arr with blank items
*/

ItemTrack* ItemTracker;
void CreateItemTracker() {
    ItemTracker = malloc(sizeof(ItemTrack*));
    ItemTracker->tree = NULL;
    ItemTracker->itemCount - 0;

}



#include <stdio.h>
Item* CreateItemEffect(float x, float y) {
	//Get Random chance generator
	float RNG = CP_Random_RangeFloat(0, 1), DropChance;
	//Get Random Type of effect
	int noEffect = 2, EType = CP_Random_RangeInt(0, noEffect), cSec = (int)CP_System_GetSeconds();
	EType = 0;
	char* StatType;
	Item* newItem = malloc(sizeof(Item));
	switch (EType) {
	//char* PStats[] = {"HEALTH", "SPEED", "DAMAGE", "FIRE RATE", "BULLET SPEED"};
	case 0: //All Base Stats Upgrade
		//newItem->AffectedBaseStat = CP_Random_RangeInt(0, NoBaseStats);
		newItem->AffectedBaseStat = 0;
		newItem->Duration = 5; //in secs
		newItem->Modifier = 10;
		newItem->Hitbox = 50;
		break;
	case 1:
		break;
	case 2:
		break;
	}
	newItem->Start = cSec;
	newItem->Type = EType;
	newItem->x = x;
	newItem->y = y;
	return newItem;
};




int ScaledEXP(Player* p) {
	double baserate = 1.1;
	int modrate = 10;
	//More exp give every modrate levels
	//every level increase exp required by baserate;
	int nRate = p->LEVEL.P_EXP % modrate == 0 ? p->LEVEL.P_EXP / 10 * baserate : baserate;

	p->LEVEL.EXP_REQ *= nRate;
	printf("Current rate %d\n", p->LEVEL.EXP_REQ);
}


void IAffectPlayer(Item* item) {
	int cSec = (int)CP_System_GetSeconds();
	if (cSec - item->Start < item->Duration) {
		switch (item->Type) {
		case 0://Affect Base Stats
			switch (item->AffectedBaseStat) {
			case 0://HP
				P.CURRENT_HP += item->Modifier;
				break;
			}
			printf("Player %s increased by %d\n", GetBaseStats(item->AffectedBaseStat), item->Modifier);
		}
	}
}


CP_Image** ItemSprites;
int Img_C;
void ItemLoadImage(void) {
	char* FilePaths[] = {
		"./Assets/Items/Base Item Sprite.png",
	};

	Img_C = (sizeof(FilePaths) / sizeof(FilePaths[0]));
	ItemSprites = malloc(sizeof(CP_Image*) * Img_C);
	for (int i = 0; i < Img_C; i++) {
		ItemSprites[i] = malloc(sizeof(CP_Image));
		ItemSprites[i] = CP_Image_Load(FilePaths[i]);
	}
}
void DrawItemImage(Item* item) {
	if (item == NULL)
		return;
	//printf("Key: %p\n",item);
	CP_Image* SImg = ItemSprites[item->Type + item->AffectedBaseStat];
	int targetScale;
	int IHeight = CP_Image_GetHeight(SImg), IWidth = CP_Image_GetHeight(SImg);
	switch (item->Type) {
	case StatBoost:
		targetScale = 40;
		CP_Image_Draw(SImg, item->x, item->y, targetScale, targetScale, IHeight, IWidth, 255);
		break;
	case EXP:
		break;
	default:
		break;
	}
}

void DrawItemTree(ItemNode* node) {
	if (node == NULL)
		return;
	//printf("\tItem: Node: %p | Key: %p\n", node, node->key);
	//DrawItemImage(&node->key);
	DrawItemImage(node->key);
	DrawItemTree(node->left);
	DrawItemTree(node->right);
}

#include <math.h>
void ItemPlayerCollision(void) {
	if (ItemTracker->tree != NULL) {
		CP_Vector target = CP_Vector_Set(P.x, P.y);
		int collected = 0;
		ItemNode* nearest = nearestNeighbour(ItemTracker->tree, target, 0);
		float dist1 = sqrt(squareDist(nearest->point.x - target.x, nearest->point.y - target.y));

		CP_Graphics_DrawLine(target.x, target.y, nearest->point.x, nearest->point.y);
	
		if (dist1 < P.HITBOX) {
			IAffectPlayer(nearest->key);
			ItemTracker->tree = deleteItemNode(ItemTracker->tree, nearest->point, 0);
			collected -= 1;
			//printf("Here\n");
		}
		//printf("Dist %f | From Player: %d\n", dist1, p->HITBOX);
	}
}

void copyItem(Item* dst, Item* src) {
	dst->AffectedBaseStat = src->AffectedBaseStat;
	dst->Duration = src->Duration;
	dst->Hitbox = src->Hitbox;
	dst->Modifier = src->Modifier;
	dst->Start = src->Start;
	dst->Type = src->Type;
	dst->x = src->x;
	dst->y = src->y;

}


ItemNode* newNode(Item *item) {
	ItemNode* result = malloc(sizeof(ItemNode));
	result->key = item;
	//result->point = CP_Vector_Set(item.x, item.y);
	result->point = CP_Vector_Set(item->x, item->y);
	result->left = NULL, result->right = NULL;
	result->prev = NULL;
	return result;
}



ItemNode* insertItemNode(ItemNode* root, Item *item) {
	return insertItemRec(NULL, root, item, 0);
}

ItemNode* insertItemRec(ItemNode* prev, ItemNode* root, Item* item, unsigned depth) {
	if (root == NULL) {
		ItemNode* n = newNode(item);
		n->prev = prev;
		return n;
	}
	unsigned cd = depth % Dimension;
	CP_Vector point = CP_Vector_Set(item->x, item->y);
	//If root->prev == NULL, root == head
	if (point.v[cd] < root->point.v[cd]) 
		root->left = insertItemRec(root, root->left, item, depth + 1);
	else
		root->right = insertItemRec(root, root->right, item, depth + 1);
	return root;
}

ItemNode* minNode(ItemNode* root, ItemNode* left, ItemNode* right, int d) {
	ItemNode* res = root;
	if (left != NULL && left->point.v[d] < res->point.v[d])
		res = left;
	if (right != NULL && right->point.v[d] < res->point.v[d])
		res = right;
	return res;
}

ItemNode* findMin(ItemNode* root, int d, unsigned int depth) {
	if (root == NULL)
		return NULL;
	unsigned cd = depth % Dimension;
	if (cd == d) {
		if (root->left == NULL)
			return root;
		return findMin(root->left, d, depth + 1);
	}
	//compare root, left, right
	return minNode(root, 
		findMin(root->left, d, depth + 1),
		findMin(root->right, d, depth + 1),
		d);
}



ItemNode* deleteItemNode(ItemNode* root, CP_Vector point, unsigned int depth) {
	if (root == NULL)
		return NULL;
	unsigned int cd = depth % Dimension;
	ItemNode* temp = NULL;
	if (point.x == root->point.x && point.y == root->point.y) {
		//if same point, check right
		if (root->right != NULL) {
			ItemNode* min = findMin(root->right, cd, 0);
			root->point = CP_Vector_Set(min->point.x, min->point.y);
			//only copy the values
			copyItem(root->key, min->key);
			temp = deleteItemNode(root->right, point, depth + 1);
			root->right = temp;
		}
		//check left
		else if (root->left != NULL) {
			ItemNode* min = findMin(root->left, cd, 0);
			root->point = CP_Vector_Set(min->point.x, min->point.y);
			//only copy the values
			copyItem(root->key, min->key);
			temp = deleteItemNode(root->left, point, depth + 1);
			root->right = temp;
			if (temp->prev == root) {
				 root->left = NULL;
			}
		}
		//check is leaf
		else {
			//free pointers
			free(root->key);
			root->key = NULL;
			free(root);
			root = NULL;
			return NULL;
		}
		return root;
	}

	if (point.v[cd] < root->point.v[cd])
		root->left = deleteItemNode(root->left, point, depth + 1);
	else
		root->right = deleteItemNode(root->right, point, depth + 1);
	return root;
}

//returns the searched item node
ItemNode* nearestNeighbour(ItemNode* root, CP_Vector point, unsigned int depth) {
	if (root == NULL)
		return NULL;

	if (root->left == NULL && root->right == NULL)
		return root;

	unsigned int cd = depth % Dimension;
	ItemNode* nextbranch, *otherbranch;
	if (point.v[cd] < root->point.v[cd]) {
		nextbranch = root->left;
		otherbranch = root->right;
	}
	else {
		nextbranch = root->right;
		otherbranch = root->left;
	}

	ItemNode* temp = nearestNeighbour(nextbranch, point, depth + 1);
	ItemNode* best = closest(temp, root, point);

	float r = squareDist(best->point.x - point.x, best->point.y - point.y);
	float rprime = point.v[cd] - root->point.v[cd];
	if (r >= squareDist(rprime, 0)) {
		temp = nearestNeighbour(otherbranch, point, depth + 1);
		best = closest(temp, best, point);
	}

	return best;
}

ItemNode* closest(ItemNode* n0, ItemNode* n1, CP_Vector point) {
	//n0 = left
	//n1 = right
	if (n0 == NULL)
		return n1;
	if (n1 == NULL)
		return n0;
	float d0 = squareDist(point.x - n0->point.x, point.y - n0->point.y);
	float d1 = squareDist(point.x - n1->point.x, point.y - n1->point.y);
	return d0 < d1 ? n0 : n1;
}



void FreeItemResource(void) {

	//for (int i = 0; i < ItemTracker->arrSize;i++) {
	//	free(ItemTracker->arr[i]);
	//}
	//free(ItemTracker->arr);

	for (int i = 0; i < Img_C; i++) {
		CP_Image* c = ItemSprites[i];
		CP_Image_Free(&(ItemSprites[i]));
		free(ItemSprites[i]);
	}
	free(ItemSprites);
    //freeTree(ItemTracker->tree);
}


void freeTree(ItemNode* root) {
	if (root == NULL)
		return NULL;
	free(root->key);
	root->key = NULL;
	freeTree(root->left);
	root->left = NULL;
	freeTree(root->right);
	root->right = NULL;
	free(root);
	root = NULL;
}



