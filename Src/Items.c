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


void IAffectPlayer(Item* i, Player* p) {

}



