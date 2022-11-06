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
    //ItemTracker->arrSize = StartItemQuantity;
    //ItemTracker->arr = malloc(sizeof(Item*) * StartItemQuantity);
    //InitItemArr(ItemTracker->arr);
    ItemTracker->tree = NULL;
    ItemTracker->itemCount - 0;

}

void InitItemArr(ItemTrack *tracker) {

//	for (int w = 0; w < tracker->arrSize;w++) {
//		//Allocate memory to it
//		tracker->arr[w] = malloc(sizeof(Item));
//		//Fill each mob pointers with data
//
//		Item* cI = tracker->arr[w];
//		cI->Hitbox = 0;
//		cI->Start = w;
//		cI->Duration = 0;
//		cI->Type = -1;
//		cI->Modifier = 0;
//		cI->x = 0;
//		cI->y = 0;
//
//	}

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
	newItem->collected = -1;
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


void IAffectPlayer(Item* item, Player* p) {
	int cSec = (int)CP_System_GetSeconds();
	if (cSec - item->Start < item->Duration) {
		switch (item->Type) {
		case 0://Affect Base Stats
			switch (item->AffectedBaseStat) {
			case 0:
				p->CURRENT_HP += item->Modifier;
				break;
			}
			printf("Player %s increased by %d\n", GetBaseStats(item->AffectedBaseStat), item->Modifier);
		}
	}
	item->collected = 1;
}





CP_Image** ItemSprites;
int Img_C;
void ItemLoadImage(void) {
	char* FilePaths[] = {
		"./Assets/Items/DmgUp.png",
	};

	Img_C = (sizeof(FilePaths) / sizeof(FilePaths[0]));
	ItemSprites = malloc(sizeof(CP_Image*) * Img_C);
	for (int i = 0; i < Img_C; i++) {
		ItemSprites[i] = malloc(sizeof(CP_Image));
		ItemSprites[i] = CP_Image_Load(FilePaths[i]);
	}
}
void DrawItemImage(Item* item) {

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
	DrawItemImage(&node->key);
	DrawItemTree(node->left);
	DrawItemTree(node->right);
}

void ItemPlayerCollision(Player*p) {
	if (ItemTracker->tree != NULL) {
		CP_Vector target = CP_Vector_Set(p->x, p->y);
		ItemNode* nearest = nearestNeighbour(ItemTracker->tree, target, 0);
		float dist1 = squareDist(nearest->point.x - target.x, nearest->point.y - target.y);
		CP_Graphics_DrawLine(target.x, target.y, nearest->point.x, nearest->point.y);
		printf("Dist %f \n", dist1);
	}
}

ItemNode* newNode(Item item) {
	ItemNode* result = malloc(sizeof(ItemNode));
	result->key = item;
	result->point = CP_Vector_Set(item.x, item.y);
	result->left = NULL, result->right = NULL;
	
}

int arePointsSame(CP_Vector p1, CP_Vector p2) {
	return p1.x == p2.x && p1.y == p2.y ? 1 : 0;
}


ItemNode* insertItemNode(ItemNode* root, Item item, unsigned depth) {
	if(root == NULL)
		return newNode(item);
	unsigned cd = depth % Dimension;
	
	CP_Vector point = CP_Vector_Set(item.x, item.y);
	if (point.v[cd] < root->point.v[cd])
		root->left = insertItemNode(root->left, item, depth + 1);
	else
		root->right = insertItemNode(root->right, item, depth + 1);
	
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

ItemNode* findMin(ItemNode* root,unsigned d, unsigned depth) {
	if (root == NULL)
		return NULL;
	unsigned cd = depth  % Dimension;
	if (cd == d) {
		if (root->left == NULL)
			return root;
		return findMin(root->left, d, depth + 1);
	}
	return minNode(root,
		findMin(root->left, d, depth + 1),
		findMin(root->right, d, depth + 1), d);
}


ItemNode* deleteItemNode(ItemNode* root, Item item, unsigned depth) {
	if (root == NULL)
		return NULL;
	
	CP_Vector point = CP_Vector_Set(item.x, item.y);
	unsigned cd = depth % Dimension;
	if (arePointsSame(point, root->point)) {
		if (root->right != NULL) {
			ItemNode* min = findMin(root->right, cd, 0);
			root->point = CP_Vector_Set(min->point.x, min->point.y);
			root->right = deleteItemNode(root->right, min->key, depth + 1);
		}
		else if (root->left != NULL) {
			ItemNode* min = findMin(root->left, cd, 0);
			root->point = CP_Vector_Set(min->point.x, min->point.y);
			root->right = deleteItemNode(root->left, min->key, depth + 1);
		}
		else {
			free(root);
			return NULL;
		}
		return root;
	}
}

//returns the searched item node
ItemNode* nearestNeighbour(ItemNode* root, CP_Vector point, unsigned depth) {
	if (root == NULL)
		return NULL;
	if (root->left == NULL && root->right == NULL)
		return root;

	unsigned cd = depth % Dimension;
	ItemNode* nextbranch, *otherbranch;
	if (point.v[cd] < root->point.v[cd]) {
		nextbranch = root->left;
		otherbranch = root->right;
	}
	else {
		nextbranch = root->right;
		otherbranch = root->left;
	}
	ItemNode* best = nearestNeighbour(nextbranch, point, depth + 1);
	ItemNode* nearest = closest(best, otherbranch, point);
	if (nearest != best)
		best = nearestNeighbour(otherbranch, point, depth + 1);

	return best;
}

ItemNode* closest(ItemNode* n0, ItemNode* n1, CP_Vector point) {
	//n0 = left
	//n1 = right
	if (n0 == NULL)
		return n1;
	if (n1 == NULL)
		return n0;
	float d0 = squareDist(point.x - n0->key.x, point.y - n0->key.y);
	float d1 = squareDist(point.x - n1->key.x, point.y - n1->key.y);
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
    freeTree(ItemTracker->tree);
}


void freeTree(ItemNode* root) {
	if (root == NULL)
		return NULL;
	freeTree(root->left);
	freeTree(root->right);
	free(root);
}





/*
//Tree Implementation
#pragma region
int TreeHeight(ItemNode* item) {
	if (NULL == item)
		return 0;
	return 1 + max(TreeHeight(item->left), TreeHeight(item->right));
}

int getBalance(ItemNode* item) {
	if (item == NULL)
		return 0;
	return TreeHeight(item->left) - TreeHeight(item->right);
}

ItemNode* newNode(Item item) {
	ItemNode* result = malloc(sizeof(ItemNode));
	result->key = item;
	result->left = NULL;
	result->right = NULL;
	result->h = 0;

}

ItemNode* insertItemNode(ItemNode* node, Item item) {
	if (node == NULL) {
		return newNode(item);
	}
	if (item.x < node->key.x)
		node->left = insertItemNode(node->left, item);
	else if (item.x > node->key.x)
		node->right = insertItemNode(node->right, item);
	else 	
		return NULL;
    node->h = TreeHeight(node);
    int balance = getBalance(node);
 
    if (balance > 1 && item.x < getX(node->left))
        return rightRotate(node);
    if (balance < -1 && item.x > getX(node->right))
        return leftRotate(node);
    if (balance > 1 && item.x > getX(node->left))
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }
	if (balance < -1 && item.x < getX(node->right))
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}
    return node;
}

ItemNode* deleteItemNode(ItemNode* root, Item key)
{ 
    if (root == NULL)
        return root;
  
    if ( key.x < root->key.x )
        root->left = deleteItemNode(root->left, key);
  
    else if( key.x > root->key.x )
        root->right = deleteItemNode(root->right, key);
    else
    {
        if( (root->left == NULL) || (root->right == NULL) )
        {
            ItemNode *temp = root->left ? root->left : root->right;
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // One child case
             *root = *temp;  
			free(temp);
        }
        else
        {
            ItemNode* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteItemNode(root->right, temp->key);
        }
    }
    if (root == NULL)
      return root;
    root->h = 1 + max(TreeHeight(root->left),
                           TreeHeight(root->right));
  
    int balance = getBalance(root);
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
  
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left =  leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

ItemNode * minValueNode(ItemNode* node)
{
    ItemNode* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

ItemNode* rightRotate(ItemNode* y) {
	ItemNode* x = y->left;
	ItemNode* T2 = x->right;
	x->right = y;
	y->left = T2;
	y->h = TreeHeight(y);
	x->h = TreeHeight(x);
	return x;
}
ItemNode* leftRotate(ItemNode* y) {
	ItemNode* x = y->right;
	ItemNode* T2 = x->left;
	x->left = y;
	y->right = T2;
	y->h = TreeHeight(y);
	x->h = TreeHeight(x);
	return x;
}

void freeTree(ItemNode* root) {
	if (NULL == root) {
		return NULL;
	}
	freeTree(root->left);
	freeTree(root->right);
	free(root);
}

float getX(ItemNode* current) {
	if (current == NULL)
		return;
	return current->key.x;
}

#pragma endregion
*/
