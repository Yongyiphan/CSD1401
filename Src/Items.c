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
	*ItemTracker = (ItemTrack){ 0 };
	//ItemTracker->ExpLL = NULL;
	//ItemTracker->expDrops = 0;
	//ItemTracker->ItemLL = NULL;
	//ItemTracker->ItemCount = 0;
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
	*newItem = EmptyItem;
	switch (EType) {
	//char* PStats[] = {"HEALTH", "SPEED", "DAMAGE", "FIRE RATE", "BULLET SPEED"};
	case EXP:
		newItem->AffectedBaseStat = expVal;
		newItem->Duration = -1;
		newItem->Modifier = pow(5, expVal + 1);
		newItem->Hitbox = 25;
		break;
	case StatBoost: //All Base Stats Upgrade
		newItem->AffectedBaseStat = CP_Random_RangeInt(0, NoBaseStats - 1);
		newItem->Duration = 2; //in secs
		newItem->Modifier = 10;
		newItem->Hitbox = 32;
		break;
	case MAGNET:
		break;
	}
	newItem->Start = cSec;
	newItem->Type = EType;
	//newItem->x = x;
	//newItem->y = y;
	newItem->coor = CP_Vector_Set(x, y);
	newItem->collected = 0;
	newItem->knockback = 5;
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
			level_up(&P.LEVEL.P_EXP, &P.LEVEL.EXP_REQ, &P.LEVEL.VAL);
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
		CP_Image_DrawSubImage(SImg, item->coor.x + pw, item->coor.y,pw, ph,
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
		CP_Image_DrawSubImage(SImg, item->coor.x, item->coor.y,pw, ph, 
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

/*
*	LINKED LIST IMPLEMENTATION
*/
#pragma region

ItemLink* DrawItemLink(ItemLink* head) {
	if (head == NULL)
		return;
	
	ItemLink* current = head, *next = NULL;
	while (current != NULL) {
	//assert(_CrtCheckMemory());
		//CP_Vector target = CP_Vector_Set(P.x - current->key->x, P.y - current->key->y);
		CP_Vector target = CP_Vector_Subtract(CP_Vector_Set(P.x, P.y), current->key->coor);
		float dist = CP_Vector_Length(target);
		if (dist < P.STATTOTAL.PICKUP_TOTAL)
			current->key->collected = -1;

		if (current->key->collected == -1) {
			float speed = dist * CP_System_GetDt() * 2, slowest = 10;
			speed = speed > slowest ? speed : slowest;
			//printf("TYPE: %s\n", current->key->Type == EXP ? "EXP" : "Stat Boost");
			CP_Vector Movement = CP_Vector_Scale(CP_Vector_Normalize(target), speed);
			if (current->key->knockback > 0){
				current->key->coor = CP_Vector_Subtract(current->key->coor, Movement);
				current->key->knockback--;
				goto ToDraw;
			}
			else {
				current->key->coor = CP_Vector_Add(current->key->coor, Movement);
			}
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
				if (current->key->applying == 0) {
					printf("Applying %s buff\n", GetBaseStats(current->key->AffectedBaseStat));
					IAffectPlayer(current->key, 1);
					current->key->Start = (int)CP_System_GetSeconds();
					current->key->applying = 1;
					goto SkipThis;
				}
			}
		}
	
		if (current->key->Type != EXP) {
			if (current->key->applying == 1) {
				if ((int)CP_System_GetSeconds() - current->key->Start > current->key->Duration) {
					//time to delete item's stat boost
					printf("Removing %s buff\n", GetBaseStats(current->key->AffectedBaseStat));
					IAffectPlayer(current->key, -1);
					next = current->next;
					deleteItemLink(&head, current->key);
					current = next;
					ItemTracker->ItemCount--;
					continue;
				}
				goto SkipThis;
			}
		}
		//only draw items that are newly initialised or not collected
	ToDraw:
		DrawItemImage(current->key);
	SkipThis:
		current = current->next;
	}

MagnetHandle:
	return head;

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
		default:
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


void FreeItemResource(void) {
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
	printf("Freeing Item Structures\n");
}

#pragma endregion

