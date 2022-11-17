#pragma once
#include "cprocessing.h"
#include <stdlib.h>
#include <math.h>
#include "Items.h"
#include "Mob.h"
#include "player.h"
#include "Map.h"
#include <assert.h>
/*
@brief		Function that initialise array with empty items
@params		tracker	-> Contains stats for tracking Items
@returns	Item arr with blank items
*/

#ifdef _DEBUG
#  define _CRTDBG_MAP_ALLOC
#  define _CRTDBG_MAP_ALLOC_NEW
#  include <crtdbg.h>
#  include <assert.h>
#endif


ItemTrack* ItemTracker;
void CreateItemTracker() {
    ItemTracker = malloc(sizeof(ItemTrack));
	ItemTracker->ExpLL = NULL;
	ItemTracker->expDrops = 0;
	ItemTracker->ItemLL = NULL;
	ItemTracker->ItemCount = 0;
	printf("Size of ItemTrack %d | Pointer: %d\n", sizeof(ItemTrack), sizeof(ItemTrack*));
	printf("Size of ItemLink %d | Pointer: %d\n", sizeof(ItemLink), sizeof(ItemLink*));
	printf("Size of Item %d | Pointer: %d\n", sizeof(Item), sizeof(Item*));
	printf("Stat\n");
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
	newItem->collected = 0;
	return newItem;
};






void IAffectPlayer(Item* item, int method) {
	int cSec = (int)CP_System_GetSeconds();
	int boost = item->Modifier * method;
	switch (item->Type) {
		case StatBoost://Affect Base Stats
		switch (item->AffectedBaseStat) {
			case 0://HP
				P.CURRENT_HP += boost;
				break;
			case 1://Movement Speed
				P.STAT.SPEED += boost;
				break;
			case 2://Damage
				P.STAT.DAMAGE += boost;
				break;
			case 3://Attack speed
				P.STAT.ATK_SPEED += boost;
				break;
			case 4://Bullet Speed
				break;
			case 5:
				P.STAT.MAX_HP += boost;
				break;
		}
		//printf("Player %s increased by %d\n", GetBaseStats(item->AffectedBaseStat), item->Modifier);
		case EXP:
			P.LEVEL.P_EXP += item->Modifier;
			//level_up(&P.LEVEL);
			//printf("Item x: %f | y: %f\n", item->x, item->y);
			break;
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
		CP_Image_DrawSubImage(SImg, item->x + pw, item->y,pw, ph,
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
/*
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
			//else
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
int deadcheck = 0;
int failedDelete = 0;
void ItemPlayerCollision(void) {
	//if (ItemTracker->exptree != NULL) {
	//	CP_Vector target = CP_Vector_Set(P.x, P.y);

	//	ItemNode * nearest = nearestNeighbour(ItemTracker->exptree, target, 0);
	//	float dist1 = CP_Math_Distance(target.x, target.y, nearest->point.x, nearest->point.y);
	//	CP_Graphics_DrawLine(target.x, target.y, nearest->point.x, nearest->point.y);
	//		
	//	if (dist1 <= P.HITBOX) {
	//		deadcheck = 0;
	//		IAffectPlayer(nearest->key, 1);
	//		printf("Removing %p | X: %f | Y: %f\n", nearest, nearest->point.x, nearest->point.y);
	//		if (deadcheck == 0) {
	//			failedDelete++;

	//			printf("\nFailed Delete\nPointer: %p | X: %f | Y: %f\n", nearest, nearest->point.x, nearest->point.y);
	//			printf("Printing Tree\n\n");
	//			//CP_Engine_Terminate();
	//		}
	//		else
	//			printf("Delete Successful %d\n", deadcheck);
	//		
	//	}
	//}
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

*/

/*
*	LINKED LIST IMPLEMENTATION
*/
#pragma region

ItemLink* DrawItemLink(ItemLink* head) {
	if (head == NULL)
		return;
	
	ItemLink* current = head, *next = NULL;
	while (current != NULL) {
	assert(_CrtCheckMemory());
		if (current->key->collected == -1) {
			goto SkipThis;
		}
		CP_Vector target = CP_Vector_Set(P.x - current->key->x, P.y - current->key->y);
		float dist = CP_Vector_Length(target);
		if ( dist <= P.STATTOTAL.PICKUP_TOTAL) {
			CP_Vector npoint = CP_Vector_Add(CP_Vector_Set(current->key->x, current->key->y), CP_Vector_Scale(CP_Vector_Normalize(target), 10));
			current->key->x = npoint.x;
			current->key->y = npoint.y;
		}
		if (dist < P.HITBOX) {
			if(current->key->Type == EXP) {
				IAffectPlayer(current->key, 1);
				next = current->next;
				deleteItemLink(&head, current->key);
				current = next;
				ItemTracker->expDrops--;
				continue;
			}
			else {
				//if collected item, start its duration timer
				IAffectPlayer(current->key, 1);
				current->key->Start = (int)CP_System_GetSeconds();
				current->key->collected = -1;
				goto SkipThis;
			}
		}
		if (current->key->Type != EXP) {
			if (current->key->collected == -1 && (int)CP_System_GetSeconds() - current->key->Start > current->key->Duration) {
				//time to delete item's stat boost
				IAffectPlayer(current->key, -1);
				next = current->next;
				deleteItemLink(&head, current->key);
				current = next;
				ItemTracker->ItemCount--;
				continue;
			}
		}
		//only draw items that are newly initialised or not collected
		DrawItemImage(current->key);
	SkipThis:
		current = current->next;
	}
	return head;
}

ItemLink* newLink(Item *item) {
	ItemLink* link = malloc(sizeof(ItemLink));
	link->key = item;
	link->next = NULL;
	return link;
}

void insertItemLink(ItemLink** head, Item* item) {
	ItemLink* nLink = newLink(item);
	switch (item->Type) {
		case EXP:
			ItemTracker->expDrops++;
			break;
		case StatBoost:
			ItemTracker->ItemCount++;
			break;
	}
	nLink->next = (*head);
	(*head) = nLink;

}
void deleteItemLink(ItemLink**head, Item* item) {
	ItemLink* curr = *head, * prev = NULL;

	if (curr != NULL && curr->key == item) {
		*head = curr->next;
		free(curr->key);
		free(curr);
		return;
	}
	while (curr != NULL && curr->key != item) {
		prev = curr;
		curr = curr->next;
	}
	if (NULL == curr)
		return;
	prev->next = curr->next;

	free(curr->key);
	free(curr);
}

void freeLink(ItemLink* head) {
	if (NULL == head)
		return NULL;
	ItemLink* tmp = NULL;
	while (head != NULL) {
		tmp = head;
		head = head->next;
		free(tmp->key);
		free(tmp);
	}

}
#pragma endregion


/*
* KD TREE IMPLEMENTATION
*/
/*
#pragma region

ItemNode* newNode(Item *item) {
	ItemNode* result = malloc(sizeof(ItemNode));
	result->key = item;
	//result->point = CP_Vector_Set(item.x, item.y);
	result->point = CP_Vector_Set(item->x, item->y);
	result->left = NULL;
	result->right = NULL;
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
		ItemTracker->expDrops++;
		return n;
	}
	unsigned cd = depth % Dimension;
	CP_Vector point = CP_Vector_Set(item->x, item->y);
	//If root->prev == NULL, root == head
	if (CP_Math_Distance((int)point.x, (int)point.y, (int)root->point.x, (int)root->point.y) <= item->Hitbox && item->Type == EXP) {
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
	if (point.x == root->point.x && point.y == root->point.y) {
		if (root->left == NULL && root->right == NULL) {
			free(root);
			root = NULL;
			deadcheck = 1;
			return NULL;
		}
		if (root->right != NULL) {
			ItemNode* min = findMin(root->right, cd, 0);
			free(root->key);
			root->key = NULL;
			root->key = min->key;

			root->point = CP_Vector_Set(min->point.x, min->point.y);
			root->right = deleteItemNode(root->right, min->point, depth + 1);
		}
		if(root->left != NULL) {
			ItemNode* min = findMin(root->left, cd, 0);
			free(root->key);
			root->key = NULL;
			root->key = min->key;

			root->point = CP_Vector_Set(min->point.x, min->point.y);
			root->right = deleteItemNode(root->left, min->point, depth + 1);
			if (root->right == root->left)
				root->left = NULL;
		}
		return root;
	}
	if(point.v[cd]<root->point.v[cd])
		root->left = deleteItemNode(root->left, point, depth + 1);
	else
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

#pragma endregion
*/

// CLEAN UP //
#pragma region
//void CleanTree(ItemNode* root) {
//	if (NULL == root)
//		return;
//
//	if (CP_Vector_Distance(CP_Vector_Set(P.x, P.y), root->point) <= 50) {
//		//ItemTracker->exptree = deleteItemNode(ItemTracker->exptree, root->point, 0);
//		failedDelete--;
//		return;
//	}
//	CleanTree(root->left);
//	CleanTree(root->right);
//}

void FreeItemResource(void) {

	//for (int i = 0; i < ItemTracker->arrSize;i++) {
	//	free(ItemTracker->arr[i]);
	//}
	//free(ItemTracker->arr);
	
	printf("Freeing Item Images\n");
	for (int i = 0; i < Img_C; i++) {
		CP_Image* c = ItemSprites[i];
		CP_Image_Free(&(ItemSprites[i]));
		free(ItemSprites[i]);
	}
	free(ItemSprites);
	freeLink(ItemTracker->ItemLL);
	freeLink(ItemTracker->ExpLL);
	free(ItemTracker);
	//assert(_CrtCheckMemory());
	printf("Freeing Item Structures\n");
}


//void freeTree(ItemNode* root) {
//	if (root == NULL)
//		return NULL;
//	free(root->key);
//	root->key = NULL;
//	freeTree(root->left);
//	root->left = NULL;
//	freeTree(root->right);
//	root->right = NULL;
//	free(root);
//	root = NULL;
//}

#pragma endregion

