#pragma once
#include "cprocessing.h"
#include <stdlib.h>
#include <math.h>
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
#pragma region
Item* CreateItemEffect(float x, float y, int exp, int expVal) {
	//Get Random chance generator
	float RNG = CP_Random_RangeFloat(0, 1), DropChance;
	//Get Random Type of effect
	int noEffect = 2, EType = CP_Random_RangeInt(1, noEffect - 1), cSec = (int)CP_System_GetSeconds();
	if(exp)
		EType = 0;
	char* StatType;
	Item* newItem = malloc(sizeof(Item));
	switch (EType) {
	//char* PStats[] = {"HEALTH", "SPEED", "DAMAGE", "FIRE RATE", "BULLET SPEED"};
	case EXP:
		newItem->AffectedBaseStat = expVal;
		newItem->Duration = -1;
		newItem->Modifier = pow(5, expVal + 1);
		newItem->Hitbox = 32;
		break;
	case StatBoost: //All Base Stats Upgrade
		newItem->AffectedBaseStat = CP_Random_RangeInt(0, NoBaseStats - 1);
		newItem->Duration = 5; //in secs
		newItem->Modifier = 10;
		newItem->Hitbox = 32;
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






void IAffectPlayer(Item* item) {
	int cSec = (int)CP_System_GetSeconds();
	if (cSec - item->Start < item->Duration || item->Duration == -1) {
		switch (item->Type) {
		case StatBoost://Affect Base Stats
			switch (item->AffectedBaseStat) {
			case 0://HP
				P.CURRENT_HP += item->Modifier;
				break;
			case 1://Movement Speed
				P.STAT.SPEED += item->Modifier;
				break;
			case 2://Damage
				P.STAT.DAMAGE += item->Modifier;
				break;
			case 3://Attack speed
				P.STAT.ATK_SPEED += item->Modifier;
				break;
			case 4://Bullet Speed
				break;
			case 5:
				P.STAT.MAX_HP += item->Modifier;
				break;
			}
			//printf("Player %s increased by %d\n", GetBaseStats(item->AffectedBaseStat), item->Modifier);
		case EXP:
			P.LEVEL.P_EXP += item->Modifier;
			level_up(&P.LEVEL.P_EXP, &P.LEVEL.EXP_REQ, &P.LEVEL.VAL);
			//printf("Item x: %f | y: %f\n", item->x, item->y);
			break;
		}
	}
}


CP_Image** ItemSprites;
int Img_C;
void ItemLoadImage(void) {
	char* FilePaths[] = {
		"./Assets/Items/EXP Sprite.png",
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
	int original_Size, scale, leftOS = 0, rightOS = 0, topOS = 0, btmOS = 0;
	CP_Image* SImg = ItemSprites[item->Type];
	int IHeight = CP_Image_GetHeight(SImg),IWidth;
	int ph = IHeight * item->Hitbox / IHeight, pw = ph;
	switch (item->Type) {
	case StatBoost:
		IWidth = CP_Image_GetWidth(SImg) / NoBaseStats;
		CP_Image_DrawSubImage(SImg, item->x, item->y,pw, ph,
			IWidth * (item->AffectedBaseStat % 5) ,
			0,
			IWidth * (item->AffectedBaseStat % 5) + IWidth,
			IHeight,
			255);
		break;
	case EXP:
		original_Size = 32;
		scale = IHeight / original_Size;
		leftOS = scale * 0, rightOS = scale * 10;
		topOS = scale * 0, btmOS = scale * 10;
		IWidth = CP_Image_GetWidth(SImg) / 3; //theres only 3 types of exp
		//Update when exp get increase as progression
		CP_Image_DrawSubImage(SImg, item->x, item->y,pw, ph, 
				item->AffectedBaseStat * IWidth + leftOS,
				topOS,
				item->AffectedBaseStat * IWidth + IWidth - rightOS,
				IHeight - btmOS,		
				255);

		break;
	default:
		break;
	}
}



int NoDeleted = 0;
void DrawItemTree(ItemNode* node) {
	if (!node)
		return;
	if (node) {
		if (node->key) {
			CP_Vector target = CP_Vector_Set(P.x - node->key->x, P.y - node->key->y);
			float dist = CP_Vector_Length(target);
			if ( dist <= P.STATTOTAL.PICKUP_TOTAL) {
				CP_Vector npoint = CP_Vector_Add(CP_Vector_Set(node->key->x, node->key->y), CP_Vector_Scale(CP_Vector_Normalize(target), 10));
				node->key->x = npoint.x;
				node->key->y = npoint.y;
				node->point = npoint;
			}
			if (dist < P.HITBOX)
				NoDeleted++;
			else
				DrawItemImage(node->key);
		}
		if(node->left)
			DrawItemTree(node->left);
		if (node->right)
			DrawItemTree(node->right);

	}
}
#define COUNT 10
void PrintTree(ItemNode* root, int space, int depth) {
	if (root == NULL)
		return;
	space += COUNT;
	char d = depth % Dimension == 0 ? 'X' : 'Y';
	PrintTree(root->right, space, depth + 1);
	printf("\n");
	for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("L(%c): %d | X: %4.3f | Y: %4.3f\n",d,depth, root->key->x, root->key->y);
 
	PrintTree(root->left, space, depth + 1);
}

#include <math.h>
void ItemPlayerCollision(void) {
	if (ItemTracker->tree != NULL) {
		CP_Vector target = CP_Vector_Set(P.x, P.y);

		ItemNode * nearest = nearestNeighbour(ItemTracker->tree, target, 0);
		float dist1 = CP_Math_Distance(target.x, target.y, nearest->point.x, nearest->point.y);
		CP_Graphics_DrawLine(target.x, target.y, nearest->point.x, nearest->point.y);
			
		if (dist1 <= P.HITBOX) {
			IAffectPlayer(nearest->key);
			ItemTracker->tree = deleteItemNode(ItemTracker->tree, nearest->point, 0);
		}
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
		ItemTracker->itemCount++;
		return n;
	}
	unsigned cd = depth % Dimension;
	CP_Vector point = CP_Vector_Set(item->x, item->y);
	//If root->prev == NULL, root == head
	if (CP_Math_Distance(point.x, point.y, root->point.x, root->point.y) <= item->Hitbox) {
		root->key->Modifier += 5;
		return root;
	}
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
	//printf("Entering root: %p X: %f | Y: %f\n", root, point.x, point.y);
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
			if (temp && temp->prev && temp->prev == root) {
				 root->left = NULL;
			}
		}
		else {
			//free pointers
			free(root->key);
			root->key = NULL;
			root->point = CP_Vector_Zero();
			free(root);
			root = NULL;
			ItemTracker->itemCount--;
			NoDeleted--;
			return NULL;
		}
		return root;
	}

	root->left =  deleteItemNode(root->left, point, depth + 1);
	root->right = deleteItemNode(root->right, point, depth + 1);
	return root;
}


// SEARCH //
#pragma region
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
	if (r > squareDist(rprime, 0)) {
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
#pragma endregion


// CLEAN UP //
#pragma region
void CleanTree(ItemNode* root) {
	if (NULL == root)
		return;

	if (CP_Vector_Distance(CP_Vector_Set(P.x, P.y), root->point) <= 50) {
		ItemTracker->tree = deleteItemNode(ItemTracker->tree, root->point, 0);
		return;
	}
	CleanTree(root->left);
	CleanTree(root->right);
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
    freeTree(ItemTracker->tree);
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

#pragma endregion

