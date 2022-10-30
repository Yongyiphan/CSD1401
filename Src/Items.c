#pragma once

#include "Items.h"
#include "player.h"
#include "cprocessing.h"






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

void GenerateItem(ItemTracker* tracker, int currentSec) {
	if (tracker->arrSize <= tracker->itemCount) {
		//Expand array
	}
	for (int i = 0; i < tracker->arrSize;i++) {
		//Check for Available Slot in arr
		if (NULL == tracker->arr[i].Type) {
			//Create Item Object
		}
		if (-1 == tracker->arr[i].Type) {
			//Update Item Object
		}
	}
}


