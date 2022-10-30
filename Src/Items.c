#pragma once

#include "Items.h"
#include "player.h"
#include "cprocessing.h"
#include <stdlib.h>
/*
@brief		Function that initialise array with empty items
@params		tracker	-> Contains stats for tracking Items
@returns	Item arr with blank items
*/

void InitItemArr(ItemTrack *tracker) {

	for (int w = 0; w < tracker->arrSize;w++) {
		//Allocate memory to it
		tracker->arr[w] = malloc(sizeof(Item));
		//Fill each mob pointers with data

		Item* cI = tracker->arr[w];
		cI->Hitbox = 0;
		cI->Start = w;
		cI->Duration = 0;
		cI->Type = -1;
		cI->Modifier = 0;
		cI->x = 0;
		cI->y = 0;

	}

}




void CreateItemEffect(Item* i) {
	//Get Random chance generator
	float RNG = CP_Random_RangeFloat(0, 1), DropChance;
	//Get Random Type of effect
	int noEffect = 2, EType = CP_Random_RangeInt(0, noEffect);
	switch (EType) {
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	}
	
};


void IAffectPlayer(Item* i, Player* p, int currentSec) {
	
	if (currentSec - i->Start < i->Duration) {
		switch (i->Type) {

		}
	}
}

void GenerateItem(ItemTrack* tracker, int currentSec) {
	if (tracker->arrSize <= tracker->itemCount) {
		//Expand array
	}
	for (int i = 0; i < tracker->arrSize;i++) {
		//Check for Available Slot in arr
		if (NULL == tracker->arr[i]->Type) {
			//Create Item Object
		}
		if (-1 == tracker->arr[i]->Type) {
			//Update Item Object
		}
	}
}


