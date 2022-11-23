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

static int blegal2 = 0, blegal3 = 0, blegal4 = 0;
ItemTrack* ItemTracker;
ItemLink* AppliedEffects = NULL;
void CreateItemTracker() {
    ItemTracker = malloc(sizeof(ItemTrack));
	*ItemTracker = (ItemTrack){ 0 };
	for (int i = 0; i < No_Items; i++) {
		ItemTracker->DropCount[i][0] = 0;
		ItemTracker->DropCount[i][1] = 0;
	}
	//Specific Drop Limitations
	ItemTracker->DropCount[MAGNET][1] = 1;
	AppliedEffects = NULL;
}
int ItemCountSum(void) {
	int s = 0;
	for (int i = 0; i < No_Items; i++)
		s += ItemTracker->DropCount[i][0];
	return s;
}
/*
ItemTracker->DropCount[] = {EXP,..., Magnet etc)
	-> [0,1]
	-> 0 = Count
	-> 1 = Limit (-1 = ignore limit)
*/

void PrintItemCount(void) {
	printf("EXP Drop count: %d | Limit: %d\n", ItemTracker->DropCount[EXP][0], ItemTracker->DropCount[EXP][1]);
	printf("Stat Drop count: %d | Limit: %d\n", ItemTracker->DropCount[StatBoost][0], ItemTracker->DropCount[StatBoost][1]);
	printf("Magnet Drop count: %d | Limit: %d\n", ItemTracker->DropCount[MAGNET][0], ItemTracker->DropCount[MAGNET][1]);
}


#include <stdio.h>
#pragma region
Item* CreateItemEffect(CP_Vector coor, int exp, int expVal) {
	//Get Random chance generator
	float RNG = CP_Random_RangeFloat(0, 1), DropChance;
	//Get Random Type of effect
	// Get Random Type of bullet
	int B_RNG = CP_Random_RangeInt(2, 4);
	int noEffect = 1, EType;
	if (exp == -1) {
		EType = CP_Random_RangeInt(1, 2);
	}
	else {
		EType = exp;
	}

	if (ItemCountSum() > 50) {
		if (ItemTracker->DropCount[MAGNET][0] < ItemTracker->DropCount[MAGNET][1]) {
			EType = MAGNET;
			//printf("Creating a magnet\n");
		}
	}

	Item* newItem = malloc(sizeof(Item));
	*newItem = EmptyItem;
	int statgen;
	switch (EType) {
	case EXP:
		newItem->AffectedBaseStat = expVal;
		newItem->Duration = -1;
		newItem->Modifier = ((float) MobCycleTimer / (P.LEVEL.VAL + 1) + P.LEVEL.VAL) * (expVal+1);
		newItem->Hitbox = 25;

		break;
	case StatBoost: //All Base Stats Upgrade
		//char* PStats[] = {"HEALTH", "SPEED", "DAMAGE", "FIRE RATE", "BULLET SPEED"};
		statgen = CP_Random_RangeInt(0, NoBaseStats - 1);
		newItem->AffectedBaseStat = statgen;
		newItem->Duration = 2; //in secs
		if (statgen == 0) {
			newItem->Duration = -1;
		}
		newItem->Modifier = 0.10;
		newItem->Hitbox = 42;
		break;
	case MAGNET:
		newItem->Duration = 2;
		newItem->Hitbox = 45;
		break;
	case COIN:
		newItem->Duration = -1;
		newItem->Hitbox = 25;
		break;
	case BULLETType:
		newItem->Duration = 5;
		newItem->Hitbox = 25;
		newItem->AffectedBaseStat = B_RNG;
	//	printf("BulletType! %d", B_RNG);
		break;

	}
	newItem->Start = MobCycleTimer;
	newItem->Type = EType;
	newItem->coor = coor;
	newItem->collected = 0;
	newItem->knockback = 2;
	return newItem;
};




void IAffectPlayer(Item* item, int method) {
	float boost = item->Modifier * method;
	switch (item->Type) {
		case StatBoost://Affect Base Stats
		switch (item->AffectedBaseStat) {
			case 0://HP
				P.CURRENT_HP += (item->Modifier * 1000);
				break;
			case 1://Movement Speed
				P.STATMULT.SPEED_MULT += boost;
				break;
			case 2://Damage
				P.STATMULT.DAMAGE_MULT += boost * 2;
				break;
			case 3://Attack speed
				P.STATMULT.ATK_SPEED_MULT += boost;
				break;
				P.STATMULT.PROJECTILE_SPD_MULT += boost;
			case 4://Bullet Speed
				break;
			case 5:
				P.STATMULT.MAX_HP_MULT += boost;
				break;
		}
		case EXP:
			P.LEVEL.P_EXP += item->Modifier;
			break;

		case BULLETType:
		switch (item->AffectedBaseStat) {
			case 2: // Bullet Spilt
				if (method == -1)
					blegal2 = 0;
				else blegal2 = 1;
				break;
			case 3: // Bullet Rocket
				if (method == -1)
					blegal3 = 0;
				else blegal3 = 1;
				break;
			case 4: // Bullet Homing
				if (method == -1)
					blegal4 = 0;
				else blegal4 = 1;
				break;
			}
			break;
	}
}


CP_Image** ItemSprites;
int Img_C;
void ItemLoadImage(void) {
	char* FilePaths[] = {
		"./Assets/Items/EXP Sprite.png",
		"./Assets/Items/Base Item Sprite.png",
		"./Assets/Items/placeholderbullet.png",
		"./Assets/Items/Magnet.png",
		"./Assets/Items/coin.png",
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
	int original_Size = 32, scale, leftOS = 0, rightOS = 0, topOS = 0, btmOS = 0;
	CP_Image* SImg = ItemSprites[item->Type];
	int IHeight = CP_Image_GetHeight(SImg),IWidth;
	int ph = IHeight * item->Hitbox / IHeight, pw = ph;
	int Displacement[3] = { -1, 0, 1 }, dx, dy;
	if (item->Dis[0] == 0 && item->Dis[1] == 0 && item->Type != EXP) {
		do {
			dx = Displacement[CP_Random_RangeInt(0, 2)];
			dy = Displacement[CP_Random_RangeInt(0, 2)];

		} while (dx != 0 && dy != 0);
		item->Dis[0] = dx;
		item->Dis[1] = dy;
		item->coor = CP_Vector_Set(item->coor.x + dx * pw, item->coor.y + dy * ph);
	}
	switch (item->Type) {
	case StatBoost:
		IWidth = CP_Image_GetWidth(SImg) / NoBaseStats;
		CP_Image_DrawSubImage(SImg, item->coor.x, item->coor.y, pw, ph,
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
		IWidth = CP_Image_GetWidth(SImg);
		CP_Image_DrawSubImage(SImg, item->coor.x, item->coor.y, pw, ph,
			0,
			0,
			IWidth,
			IHeight,
			255);
		break;
	}
}


ItemLink* GetCurrentEffects(void) {
	return AppliedEffects;
}

void UpdateAppliedEffects(Item* item) {
	int found = 0;
	ItemLink* head;
	head = AppliedEffects;
	if (item == NULL)
		goto UpdateEffects;
	while (head != NULL) {
		if (item->Type == head->key->Type && item->AffectedBaseStat == head->key->AffectedBaseStat) {
			float durationleft = head->key->Duration - ( MobCycleTimer - head->key->Start);
			head->key->Duration = item->Duration;
			head->key->Start = MobCycleTimer;
			found = 1;
			//Found existing applied effect, break, time for next loop -> iter == 1
			break;
		}
		head = head->next;
	}
	if(found == 0){
		//item not currently being applied
		ItemLink* nLink = malloc(sizeof(ItemLink));
		Item* nitem = malloc(sizeof(Item));
		//nitem = &(Item) { 0 };
		nitem->Type = item->Type;
		nitem->AffectedBaseStat = item->AffectedBaseStat;
		nitem->Duration = item->Duration;
		nitem->Start = item->Start;
		nLink->key = nitem;
		nLink->next = AppliedEffects;
		AppliedEffects = nLink;
	}

UpdateEffects:
	while (head != NULL) {
		float timeLeft = head->key->Duration - ( MobCycleTimer - head->key->Start);
		if (timeLeft <= 0) {
			ItemLink* next = head->next;
			deleteItemLink(&AppliedEffects, head->key);
			head = next;
			continue;
		}
		//head->key->Start = MobCycleTimer;
		head = head->next;
	}
}

void DrawAppliedEffects() {
	ItemLink* head = AppliedEffects;
	/*
	* Btm Right = (WWidth, WHeight)
	* Draw Sprite at btm right left to right by order of application
	* Draw current duration
	* Use For loop, easier to control
	*/
	CP_Image SImg;
	int iconsize = 64, nx = P.coor.x + (CP_System_GetWindowWidth()/ 2) - iconsize, ny = P.coor.y + (CP_System_GetWindowHeight() /2 ) - iconsize /2;
	int IHeight, IWidth, SpriteIndex;
	//CP_Settings_ImageMode(CP_POSITION_CENTER);
	for (;head != NULL; head = head->next, nx -= iconsize) {
		SImg = ItemSprites[head->key->Type];
		IHeight = CP_Image_GetHeight(SImg);
		switch (head->key->Type) {
		case StatBoost:
			IWidth = CP_Image_GetWidth(SImg) / NoBaseStats, SpriteIndex = head->key->AffectedBaseStat % NoBaseStats;
			CP_Image_DrawSubImage(SImg, nx, ny, iconsize, iconsize,
				IWidth * SpriteIndex,
				0,
				IWidth * SpriteIndex + IWidth,
				IHeight,
				255);
			break;
		default:
			IWidth = CP_Image_GetWidth(SImg);
			CP_Image_DrawSubImage(SImg, nx, ny, iconsize, iconsize,0,0,IWidth,IHeight,255);
			break;
		}
		CP_Settings_StrokeWeight(0);
		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Settings_Fill(CP_Color_Create(0,0,0,80));
		float timeLeft = head->key->Duration - ( MobCycleTimer - head->key->Start);
		float nheight = (timeLeft / (float) head->key->Duration) * iconsize;
		CP_Graphics_DrawRect(nx, ny + (iconsize/2), iconsize, nheight);
		char buffer[255];
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
		sprintf_s(buffer, 255, "%1.0f", timeLeft);
		CP_Font_DrawText(buffer, nx, ny - (iconsize * 2/ 3));
	}
}

void CheckItems(void) {
	if (ItemTracker->ItemLL != NULL) {
		ItemTracker->ItemLL = ItemInteraction(ItemTracker->ItemLL);
	}
	if (ItemTracker->ExpLL != NULL) {
		ItemTracker->ExpLL = ItemInteraction(ItemTracker->ExpLL);
	}
	if (ItemTracker->CoinLL != NULL) {
		ItemTracker->CoinLL = ItemInteraction(ItemTracker->CoinLL);
	}
}

/*
*	LINKED LIST IMPLEMENTATION
*/
#pragma region

int IsMagnet = 0, ToCollect = 0;
ItemLink* ItemInteraction(ItemLink* head) {
	if (head == NULL)
		return;
	
	ItemLink* current = head, *next = NULL;
	//assert(_CrtCheckMemory());
	while (current != NULL) {
		int CType = current->key->Type;
		if (CType == MAGNET && current->key->applying == 1) {
			//printf("IM MAGNET.\n");
			IsMagnet = 1;
			P.STATTOTAL.PICKUP_TOTAL = 3000;
			//CP_Graphics_DrawCircle(P.x, P.y, P.STATTOTAL.PICKUP_TOTAL);
		}
		CP_Vector target = CP_Vector_Subtract(CP_Vector_Set(P.x, P.y), current->key->coor);
		float dist = CP_Vector_Length(target);
		if (dist < P.STATTOTAL.PICKUP_TOTAL)
			current->key->collected = -1;
		if (current->key->collected == -1) {
			float speed = dist * CP_System_GetDt() * 2;
			//speed = speed > slowest ? speed : slowest;
			CP_Vector Movement = CP_Vector_Scale(CP_Vector_Normalize(target), speed * (P.STATTOTAL.SPEED_TOTAL / 100));
			if (current->key->knockback > 0) {
				current->key->coor = CP_Vector_Subtract(current->key->coor, CP_Vector_Scale(Movement, 2));
				current->key->knockback--;
			}
			if (dist < P.STAT.PICKUP && dist > P.HITBOX) {
				Movement = CP_Vector_Scale(Movement, 1.5);
			}
			current->key->coor = CP_Vector_Add(current->key->coor, Movement);
		}
		/* ITEM DELETE CONDITIONS
		* -> WITHIN P HITBOX
		* -> ITEM EFFECTS ENDED
		*  ITEM DRAWING CONDITIONS
		* -> OUTSIDE P HITBOX
		* -> ITEM NOT APPLYING EFFECTS
		*/
		if (dist < P.HITBOX) {
			if(CType == EXP) {
				IAffectPlayer(current->key, 1);
				next = current->next;
				deleteItemLink(&head, current->key);
				current = next;
				ItemTracker->DropCount[EXP][0]--;
				continue;
			}
			else {
				//if collected item, start its duration timer
				if (current->key->Duration == -1) {
					IAffectPlayer(current->key, 1);
					next = current->next;
					deleteItemLink(&head, current->key);
					current = next;
					ItemTracker->DropCount[CType][0]--;
					if (CType == COIN)
						P.STAT.Coin_Gained += 5;
					continue;
				}
				if (current->key->applying == 0) {

					IAffectPlayer(current->key, 1);
					current->key->Start = MobCycleTimer;
					current->key->applying = 1;
					UpdateAppliedEffects(current->key);
				}
			}
		}
	
		int timeDiff = MobCycleTimer - current->key->Start;
		if (CType != EXP) {
			if (timeDiff > current->key->Duration && current->key->applying == 1) {
				//time to delete item's stat boost
				if (CType == MAGNET) {
					//printf("IM NOT MAGNET.\n");
					IsMagnet = 0;
				}
				IAffectPlayer(current->key, -1);
				next = current->next;
				deleteItemLink(&head, current->key);
				current = next;
				ItemTracker->DropCount[CType][0]--;
				UpdateAppliedEffects(NULL);
				continue;
			}
		}


		//only draw items that are newly initialised or not collected
		if (current->key->collected = -1 && current->key->applying != 1) {
			DrawItemImage(current->key);
		}
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
	ItemTracker->DropCount[item->Type][0]++;
	//PrintItemCount();
	//Maintain Magnet Item as head of linkedlist
	if ((*head) != NULL && (*head)->key->Type == MAGNET) {
		ItemLink* cnext = (*head)->next;
		(*head)->next = nLink;
		nLink->next = cnext;
	}
	else {
		nLink->next = (*head);
		(*head) = nLink;
	}

}

void deleteItemLink(ItemLink**head, Item* item) {
	ItemLink* curr = *head, * prev = NULL;

	if (curr != NULL && curr->key == item) {
		*head = curr->next;
		//printf("Freeing: %p | %p\n", curr, curr->key);
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

	//printf("Freeing: %p | %p\n", curr, curr->key);
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
	freeLink(ItemTracker->ExpLL);
	freeLink(ItemTracker->ItemLL);
	freeLink(AppliedEffects);
	free(ItemTracker);
	printf("Freeing Item Structures\n");
}

int Bulletlegal(int i)
{
	switch (i) {
	case 2:
		if (blegal2 == 1)
			return 1;
		break;
	case 3:
		if (blegal3 == 1)
			return 1;
		break;
	case 4:
		if (blegal4 == 1)
			return 1;
		break;
	default:
		return 0;
	}
}

