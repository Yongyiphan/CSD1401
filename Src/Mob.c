#pragma once
#include "cprocessing.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <utils.h>
#include "Mob.h"
#include "Map.h"
const double PI = 22.0 / 7.0;

/* --------------------------------------
* File Level Documentation
* @author	Edgar Yong
* @email	y.yiphanedgar.digipen.edu
* @file		Mob.c
* @brief	This file contains all functions required for Mob Spawning during game runtime.
			-> Initialises, generate mob objects, draw mob objects
			-> Pathing for different mobs
			-> Mob and player iteraction
			-> Free All resources created through malloc

* Copyright 2022 Digipen, All Rights Reserved.
*//*-------------------------------------*/




//Pool of Mob Types
//Edit here for different Mob Types
/*
@brief		Function to update mob's base stats based off type given
@params		cStat	-> current mob's base stats
			type	-> types of values to update with.
@returns	Updated base stats with new values
*/
int MobCosts[MobTypes] = { 1,2};
float statscale;
void CreateBaseStat(MobStats* cStat, int type)
{
    switch (type) {
		//Mob Format:
		//int HP;
		//int DEF;
		//int Speed;

		//int Range;
		//int Dmg;
		//int size;
		case SmallMob:
			cStat->HP = 5 * statscale * 2;
			cStat->Speed = 5 + statscale;
			cStat->Dmg = 1 + statscale;
			cStat->size = 50;
			break;
		case MediumMob:
			cStat->HP = 15 * statscale * 2;
			cStat->Speed = 2;
			cStat->Dmg = 2 + statscale;
			cStat->size = 60 * statscale > 200 ? 200 : 60 * statscale;
			break;
		default:
			//Something Might be wrong;
			cStat->HP = 5;
			cStat->Speed = 5;
			cStat->Range = 0;
			cStat->Dmg = 1;
			cStat->size = 200;
			break;
	}
}


/*
@brief		Function to Update mob with relevant information
@brief		m	-> target mob to update values with
			Base	-> base stats generated
			Player	-> used to determine where mobs can be spawn
			offset	-> radius of which mobs cannot be spawned in
@return		Updated Mob struct
*/
void CreateMob(Mob*m, MobStats *Base, Player*player, int offSet)
{
	float MaxRadius = (float) WHeight * BoundScale;
	float nx, ny, rTheta, r, Diff = 0.0;
	rTheta = CP_Random_RangeFloat(0, 1) * 2 * PI;
	r = sqrt(CP_Random_RangeFloat(0, 1) * (squareDist(MaxRadius,0) - squareDist(offSet, 0)) + squareDist(offSet,0));
	nx = player->x + r * cos(rTheta);
	ny = player->y + r * sin(rTheta);
	m->coor = CP_Vector_Add(CP_Vector_Scale(CP_Vector_Set(cos(rTheta), sin(rTheta)), r), player->coor);
	m->dest = player->coor;
	m->BaseStats = *Base;
	m->CStats = *Base;
	m->Status = 1;
}


int MobCycleTimer = 0;
int WaveIDQueue[NO_WAVES], CWave, MaxMob, CWaveCost;
int MobCount[NO_WAVES];
WaveTrack WaveTracker[NO_WAVES];
void CreateWaveTracker(void) {
	CWave = 0, MaxMob = MaxMobGrowthRate, CWaveCost = WaveCostGrowthRate;
	MobCycleTimer = 0, statscale = 1.1;
	for (int i = 0; i < NO_WAVES; i++) {
		WaveTracker[i] = (WaveTrack){
			MaxMob, //Max Mob
			0, //MobCount
			0, //Current Mob Count
			0, //Wave Cost
			StartMobQuantity, //Array Size 
			SpawnAreaOffset, //Spawn offset
			0, //Start Timer;
			malloc(sizeof(Mob*) * StartMobQuantity) //, //Arr
		};
		InitWavesArr(&WaveTracker[i], 0);
		WaveIDQueue[i] = -1;
		MobCount[i] = 0;
	}
}

void InitWavesArr(WaveTrack* tracker, int start) {

	MobStats bs = (MobStats){0};
	for (int w = start; w < tracker->arrSize; w++) {
		//Allocate memory to it
		tracker->arr[w] = malloc(sizeof(Mob));
		//Fill each mob pointers with data
		*tracker->arr[w] = (Mob){ 0 };
		tracker->arr[w]->Title = w;
		tracker->arr[w]->BaseStats = bs;
		tracker->arr[w]->CStats = bs;
		//Dead = 0, Alive = 1, Blank = 2
		tracker->arr[w]->Status = 2;
		tracker->arr[w]->coor = CP_Vector_Zero();
	}
}


void GenerateMobs(WaveTrack* tracker, Player* p) {
	int MobC = 0, cost = tracker->WaveCost, randM, randMCost;
	while (cost > 0) {
		if (MobC == tracker->MaxMob) {
			break;
		}
		randM = CP_Random_RangeInt(0, 1);
		randMCost = MobCosts[randM];		//Expand array
		if (MobC >= tracker->arrSize) {
			int nQ = tracker->arrSize * 2;
			Mob** tArr = realloc(tracker->arr,sizeof(Mob*) * nQ);
			if (NULL == tArr) {
				return NULL;
			}
			tracker->arr = tArr;
			tracker->arrSize = nQ;
			InitWavesArr(tracker, MobC);
			printf("\tNew Arr Size = %d", nQ);
		}

		if (cost >= randMCost) {
			Mob* cMob = tracker->arr[MobC];
			cMob->Title = randM;
			CreateBaseStat(&cMob->BaseStats, randM);
			CreateMob(cMob, &cMob->BaseStats, p, tracker->spawnOffset);
			
		}
		cost -= randMCost;
		MobC += 1;
	}
	tracker->MobCount = MobC;
	tracker->CurrentCount = MobC;
}
void GenerateWaves(void) {
		static int TimesUP = 0;
		if (MobCycleTimer >= BIGNONO) {
			if (MobCycleTimer % BIGNONO == 0 && TimesUP == 0) {
				statscale *= 5;
				TimesUP = 1;
			}
			if (MobCycleTimer % BIGNONO == 1 && TimesUP == 1) {
				TimesUP = 0;
			}
		}
		static int DifficultyBL = 0;
		if (MobCycleTimer % Wave_Timer == 0) {
			DifficultyBL++;
		}
		if (MobCycleTimer % Spawn_Timer == 0) {
			for (int i = 0; i < NO_WAVES; i++)
			{
				//At default WaveIDQueue = {-1,-1,-1,-1}
				//Whereby each "-1" == to available slot to generate waves
				if (WaveIDQueue[i] == -1) {
					++CWave; //Increment WaveCount
					if (CWave % NO_WAVES == 0 && DifficultyBL > 0) {
						DifficultyBL--;
						statscale *= MobStatScale;
						MaxMob += MaxMobGrowthRate;
					}
					if (MaxMob >= (MaxUpperLimit / NO_WAVES)) {
						WaveTracker[i].MaxMob = (MaxUpperLimit / NO_WAVES);
						WaveTracker[i].WaveCost = CWaveCost;
					}
					else {
						WaveTracker[i].MaxMob = MaxMob;								//Update Max Mob limit
						WaveTracker[i].WaveCost = CWaveCost += WaveCostGrowthRate;	//Update Value which allows spawning of mobs
					}
					//Generate Waves at avaiable slot 
					GenerateMobs(&WaveTracker[i], &P);
					//Edit increment to spawn more mob each waves
					WaveIDQueue[i] = CWave; //Update waves of queue at [i]
					MobCount[i] = WaveTracker[i].MobCount;
					break;
				}
			}
		}
		return;
}


/*
@brief		Function To load images
@params		Sprites	-> array that contains sprites for mobs
			Size	-> Size of array
@returns	Filled up array of pointers to CP_Image Objs
*/
CP_Image** MobSprites = NULL;
int Mob_Img;
void MobLoadImage(void) {
	char* FilePaths[] = {
		"./Assets/Mobs/SmallM.png",
		"./Assets/Mobs/SmallM_Flipped.png",
		"./Assets/Mobs/RangeM.png",
		"./Assets/Mobs/RangeM_Flipped.png"
	};
	Mob_Img = (sizeof(FilePaths) / sizeof(FilePaths[0]));
	if (MobSprites == NULL) {
		MobSprites = malloc(sizeof(CP_Image*) * Mob_Img);
		for (int i = 0; i < Mob_Img; i++) {
			MobSprites[i] = malloc(sizeof(CP_Image));
			MobSprites[i] = CP_Image_Load(FilePaths[i]);
		}
	}
}
void DrawMobImage(Mob* m, Player* p) {
	int IHeight, IWidth, alpha = 255,original_Size, scale, leftOS = 0, rightOS = 0, topOS = 0, btmOS = 0;
	int SizeDef = 5, StartImgI = m->Title * 2, flip = 0;
	int FrameStep = 0, targetFPS = 6;
	int u0 = 0, v0 = 0, u1 = 0, v1 = 0;
	if(CP_Vector_Distance(m->coor, p->coor) <= p->HITBOX){
		//Dying
		alpha = (m->CStats.HP / m->BaseStats.HP) * 255;
	}
	if (m->coor.x > p->coor.x) {
		StartImgI += 1;
		flip = 1;
	}
	CP_Image* SImg = MobSprites[StartImgI];
	IHeight = CP_Image_GetHeight(SImg);
	int h, w;
	float t;
	switch (m->Title) {
		case SmallMob:
			original_Size = 32;
			SizeDef = 5, targetFPS = 6;
			scale = IHeight / original_Size;
			IWidth = CP_Image_GetWidth(SImg) / SizeDef;

			leftOS = scale * 9, rightOS = scale * 6;
			if (flip == 1) SWAP(leftOS, rightOS);
			topOS = scale * 7, btmOS = scale * 5;
			
			if (m->h == 0 && m->w == 0) {
				h = original_Size * scale - topOS - btmOS;
				w = original_Size * scale - leftOS - rightOS;
				t = (float) h/ (float)w;
				m->h = m->BaseStats.size;
				m->w = m->BaseStats.size * t;
			}

			FrameStep = (m->AnimationCycle / targetFPS) % SizeDef;
			
			CP_Image_DrawSubImage(SImg, m->coor.x, m->coor.y,m->w, m->h, 
				FrameStep * IWidth + leftOS,
				topOS,
				FrameStep * IWidth + IWidth - rightOS,
				IHeight - btmOS,		
				alpha);
			break;
		case MediumMob:
			original_Size = 32;
			SizeDef = 2, targetFPS = 4;
			scale = IHeight / original_Size;
			IWidth = CP_Image_GetWidth(SImg) / SizeDef;

			leftOS = scale * 8, rightOS = scale * 7;
			if (flip == 1) SWAP(leftOS, rightOS);
			topOS = scale * 6, btmOS = scale * 4;
			if (m->h == 0 && m->w == 0) {
				h = original_Size * scale - topOS - btmOS;
				w = original_Size * scale - leftOS - rightOS;
				t = (float) h/ (float)w;
				m->h = m->BaseStats.size;
				m->w = m->BaseStats.size * t;
			}
			FrameStep = (m->AnimationCycle / targetFPS) % SizeDef;
			CP_Image_DrawSubImage(SImg, m->coor.x, m->coor.y, m->w, m->h,
				FrameStep * IWidth + leftOS,
				topOS,
				FrameStep * IWidth + IWidth - rightOS,
				IHeight - btmOS,		
				alpha);
			break;
		default:
			break;
	}
}


float squareDist(float one, float two) {
	if (two == 0)
		return one * one;
	 return one * one + two * two;
}

void MobMovement(Mob* m) {
	if (m->h > 0) {
		int status = 0;
		float dMtoP = squareDist(P.x - m->coor.x, P.y - m->coor.y);
		CP_Vector NormBase = CP_Vector_Normalize(CP_Vector_Subtract(P.coor, m->coor));
		CP_Vector BasePF = CP_Vector_Scale(NormBase, m->CStats.Speed);
		int duration;
		float maxDistanceDelta, mag;
		switch (m->Title) {
		case SmallMob:
			break;
		case MediumMob:
			duration = 5;
			if (dMtoP < squareDist(CP_System_GetWindowWidth() / 2, 0)) {
				if (m->AnimationCycle % 80 == 0) {
					m->boost = 1;
					m->dest = P.coor;
				}
				if (m->boost == 1) {
					maxDistanceDelta = 450 * CP_System_GetDt();
					mag = CP_Vector_Distance(m->dest, m->coor);
					if (mag <= maxDistanceDelta|| mag == 0.0f) {
						m->boost = 0;
						m->dest = P.coor;
					}
					else {
						status = 1;
						m->coor = CP_Vector_Add(m->coor, CP_Vector_Scale(CP_Vector_Normalize(CP_Vector_Subtract(m->dest, m->coor)), maxDistanceDelta));
						return;
					}
				}
			}
			break;
		}
		if (0 == status) {
			m->coor = CP_Vector_Add(m->coor, BasePF);
		}
	}
}

void MobTPlayerCollision(Mob* m, Player* p) {
	
	if (CP_Vector_Length(CP_Vector_Subtract(m->coor, p->coor)) <= p->HITBOX) {
		//m->CStats.HP -= p->STATTOTAL.DAMAGE_TOTAL / 4;
		p->CURRENT_HP -= m->CStats.Dmg;
	}
	if (m->CStats.HP <= 0) {
		m->Status = 0;
	}
}

 


void PrintWaveStats(void) {
	//Result Print Start
	printf("Current Max Mob: %5d | Current Cost: %5d\n", MaxMob, CWaveCost);
	printf("\nCurrent Wave: %d\n%10s: ", CWave, "Wave Queue");
	for (int i = 0; i < NO_WAVES; i++) {
		printf("| %4d ", WaveIDQueue[i]);
	}
	printf("\n%10s: ", "Mob Count");
	int tMob = 0;
	for (int i = 0; i < NO_WAVES; i++) {
		printf("| %4d ", MobCount[i]);
		tMob += MobCount[i];
	}
	printf(" |Total: %4d\n", tMob);
	//Result Print End
}

void FreeMobResource(void) {
	for (int i = 0; i < NO_WAVES; i++) {
		for (int a = 0; a < WaveTracker[i].arrSize; a++) {
			free(WaveTracker[i].arr[a]);
			WaveTracker[i].arr[a] = NULL;
		}
		free(WaveTracker[i].arr);
		WaveTracker[i].arr = NULL;
	}
	for (int i = 0; i < Mob_Img; i++) {
		CP_Image_Free(&(MobSprites[i]));
		free(MobSprites[i]);
		MobSprites[i] = NULL;
	}
	free(MobSprites);
	MobSprites = NULL;

}