#pragma once
#include "cprocessing.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "player.h"
#include "Mob.h"

//#include "player.h"

#define MaxUpperLimit 4000
#define Img_Scale 80
#define BoundScale 1.2f




//Pool of Mob Types
//Edit here for different Mob Types
/*
@brief		Function to update mob's base stats based off type given
@params		cStat	-> current mob's base stats
			type	-> types of values to update with.
@returns	Updated base stats with new values
*/
int MobCosts[MobTypes] = { 1,2,3, 4, 5};
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
			cStat->HP = 5;
			cStat->DEF = 10;
			cStat->Speed = 5;
			cStat->Range = 0;
			cStat->Dmg = 1;
			cStat->size = 10;
			break;
		case MediumMob:
			cStat->HP = 5;
			cStat->DEF = 10;
			cStat->Speed = 5;
			cStat->Range = 0;
			cStat->Dmg = 1;
			cStat->size = 15;
			break;
		case BigMob:
			cStat->HP = 5;
			cStat->DEF = 10;
			cStat->Speed = 5;
			cStat->Range = 0;
			cStat->Dmg = 1;
			cStat->size = 25;
			break;
		case RangedMob:
			cStat->HP = 5;
			cStat->DEF = 10;
			cStat->Speed = 5;
			cStat->Range = 0;
			cStat->Dmg = 1;
			cStat->size = 10;
			break;
		case BigBoss:
			cStat->HP = 5;
			cStat->DEF = 10;
			cStat->Speed = 5;
			cStat->Range = 0;
			cStat->Dmg = 1;
			cStat->size = 50;
			break;
		default:
			//Something Might be wrong;
			cStat->HP = 5;
			cStat->DEF = 10;
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
#include <math.h>
const double PI = 22.0 / 7.0;
int wHeight, wWidth;
void CreateMob(Mob*m, MobStats *Base, Player*player, int offSet)
{
	float MaxRadius = (float) CP_System_GetWindowHeight() * BoundScale;
	//Uncomment below if you want to manually set spawn radius
	//MaxRadius = 600;
	//Assume Player center of spawnable area
	float nx, ny, rTheta, r, Diff = 0.0;
	//do {
	//} while (Diff <= offSet);
		/*Formula for generating points in circle*/
	rTheta = CP_Random_RangeFloat(0, 1) * 2 * PI;
	//eqn = sqrt( random() * (MaxRadius**2 - MinRadius**2) + MinRadius**2 ) <- MinRadius == offSet
	r = sqrt(CP_Random_RangeFloat(0, 1) * (square(MaxRadius, MaxRadius) - square(offSet, offSet)) + square(offSet, offSet));
	nx = player->x + r * cos(rTheta);
	ny = player->y + r * sin(rTheta);


	m->BaseStats = *Base;
	m->CStats = *Base;
	m->x = nx;
	m->y = ny;
	m->Status = 1;
	m->AnimationCycle = 0;
	m->w = 0;
	m->h = 0;
}





///*
//@brief		Initialise Mob arrays with Blank Mobs with blank MobStats
//@params		WaveTrack struct pointer to be filled with blank mobs
//@returns	WaveTrack's array to be filled with blank mobs (to be modified/updated when needed)
//*/
void InitWavesArr(WaveTrack* tracker, int start) {
	wHeight = CP_System_GetWindowHeight(), wWidth = CP_System_GetWindowWidth();
	MobStats bs = (MobStats){
		0,0,0,0,0,0
	};

	for (int w = start; w < tracker->arrSize; w++) {
		//Allocate memory to it
		tracker->arr[w] = malloc(sizeof(Mob));
		//Fill each mob pointers with data
		tracker->arr[w]->Title = w;
		tracker->arr[w]->BaseStats = bs;
		tracker->arr[w]->CStats = bs;
		tracker->arr[w]->x = 0;
		tracker->arr[w]->y = 0;
		//Dead = 0, Alive = 1, Blank = 2
		tracker->arr[w]->Status = 2;
		tracker->arr[w]->AnimationCycle = 0;
		tracker->arr[w]->w = 0;
		tracker->arr[w]->h = 0;
	}
}


////Function that generated Mobs based off a cost system
///*
//@brief		Generates Mobs, Fills up a single array with mobs
//@params		tracker ->	a struct containing wave details, 
//						wave array for mobs and 
//						spawn offset area around player
//			player	->	contains player's coordinates
//@returns	Fills in a array of Mob pointers pointing to blank mob objects (created in InitWaveArr())
//*/
void GenerateMobs(WaveTrack* tracker, Player* p) {
	int MobC = 0, cost = tracker->WaveCost, randM, randMCost;
	//MobNode* root = tracker->tree, *current;
	
	while (cost > 0) {
		if (MobC == tracker->MaxMob) {
			break;
		}
		//randM = CP_Random_RangeInt(0, 1);
		randM = 0;
		randMCost = MobCosts[randM];		//Expand array
		if (MobC >= tracker->arrSize) {
			int nQ = tracker->arrSize * 2;
			Mob** tArr = realloc(tracker->arr,sizeof(Mob*) * nQ);
			if (NULL != tArr) {
				tracker->arr = tArr;
				tracker->arrSize = nQ;
			}
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
	//for (int i = 0; i < MobC; i++) {
	//	tracker->tree = insert(tracker->tree, *tracker->arr[i]);
	//}

	//free(tarr);

	tracker->MobCount = MobC;
	tracker->CurrentCount = MobC;
}

/*
@brief		Generates Waves at intervals to spanw mobs
@params		Player	-> Contains reference to player struct (used for generated mobs)
			WaveTrack queue ->	Contains an array (1) of array of pointers to mobs structs
								Each array (1) refering to a wave of mobs
			queueID	-> Array of int (same size as queue) which array (1) of queue is empty to generate mobs in
			WavesNo	-> Size of queue & queueID
			CostGrowth	-> The cost which algo can spend to generate mobs for waves
			MaxMobGrowth	-> Number which limits the maximum number of mobs that can be generated per wave
			WaveCount	-> Current Wave number
			MobCount	-> Array of Int (same size as queue) that contains mob count for every current waves of mobs stores in queue
@returns	If theres an empty slot in queueID
				Fill array at that position in queue with mobs based of CostGrowth, Limited by MaxMobGrowth
				Updates WaveCount
				Updates MobCount
			Else:
				Nothing Happens
*/
void GenerateWaves(Player*P, WaveTrack* queue, int* queueID, int WavesNo, int CostGrowth, int MaxMobGrowth, int* WaveCount, int*MobCount) {
	for (int i = 0; i < WavesNo; i++) {
		//At default WaveIDQueue = {-1,-1,-1,-1}
		//Whereby each "-1" == to available slot to generate waves
		if (queueID[i] == -1) {
			*WaveCount += 1; //Increment WaveCount
			if (MaxMobGrowth >= (MaxUpperLimit / WavesNo)) {
				queue[i].MaxMob = (MaxUpperLimit / WavesNo);
			}
			else {
				queue[i].MaxMob = MaxMobGrowth;//Update Max Mob limit
				queue[i].WaveCost = CostGrowth;	//Update Value which allows spawning of mobs
			}
			//Generate Waves at avaiable slot 
			GenerateMobs(&queue[i], P);
			//printf("\n\tCreated Wave: %d\n", *WaveCount);
			//Edit increment to spawn more mob each waves
			queueID[i] = *WaveCount; //Update waves of queue at [i]
			MobCount[i] = queue[i].MobCount;
			
			//Wave Color
			//Assign random color to each wave
			//queue[i].waveColor = CP_Color_Create(CP_Random_RangeInt(100, 255), CP_Random_RangeInt(100, 255), CP_Random_RangeInt(100, 255), 255);
			break;
		}
	}
}



/*
@brief		Function To load images
@params		Sprites	-> array that contains sprites for mobs
			Size	-> Size of array
@returns	Filled up array of pointers to CP_Image Objs
*/
void MobLoadImage(CP_Image* Sprites, int No_Img) {
	char *FilePaths[] = {
		"./Assets/SmallM.png",
		"./Assets/SmallM_Flipped.png",
		"./Assets/RangeM.png",
		"./Assets/RangeM_Flipped.png",
	};
	/*
	(Walk = +0, Flipped = +1)
	Small Mob => 0 ~ 1
	Medium Mob => 2 ~ 3
	Big Mob => 4 ~ 5
	*/

	int Img_C = (sizeof(FilePaths) / sizeof(FilePaths[0]));
	if (Img_C == No_Img) {
		for (int i = 0; i < No_Img; i++) {
			Sprites[i] = malloc(sizeof(CP_Image));
			Sprites[i] = CP_Image_Load(FilePaths[i]);
		}
	}
	else {
		printf("Error: Image and arr size don't match\n");
	}

}
void DrawMobImage(CP_Image* Sprites, Mob* m, Player* p) {

	int IHeight, IWidth, alpha = 255;
	int SizeDef = 5, StartImgI = m->Title * 2;
	int FrameStep = 0, targetFPS = 6;
	m->AnimationCycle += 1;
	int u0 = 0, v0 = 0, u1 = 0, v1 = 0;
	/*
	SM = 0, 0 * 2 = 0
	MM = 1, 1 * 2 = 2
	BM = 2, 2 * 2 = 4
	*/
	if (CP_Math_Distance(m->x, m->y, p->x, p->y) <= p->HITBOX) {
		//Dying
		alpha = (m->CStats.HP / m->BaseStats.HP) * 255;
	}
	if (m->x > p->x) {
		StartImgI += 1;
	}
	CP_Image* SImg = Sprites[StartImgI];
	IHeight = CP_Image_GetHeight(SImg);

	switch (m->Title) {
		case SmallMob:
			SizeDef = 5, targetFPS = 6;
			IWidth = CP_Image_GetWidth(SImg) / SizeDef;
			m->h = IHeight * Img_Scale / IHeight, m->w = m->h;

			FrameStep = (m->AnimationCycle / targetFPS) % SizeDef;
			u0 = FrameStep * IWidth, v0 = 0, u1 = FrameStep * IWidth + IWidth, v1 = IHeight;
			CP_Image_DrawSubImage(SImg, m->x, m->y,m->w, m->h, u0, v0, u1, v1, alpha);

			break;
		case MediumMob:
			SizeDef = 2, targetFPS = 4;
			IWidth = CP_Image_GetWidth(SImg) / SizeDef;
			m->h = IHeight * Img_Scale / IHeight, m->w = m->h;

			FrameStep = (m->AnimationCycle / targetFPS) % SizeDef;
			u0 = FrameStep * IWidth, v0 = 0, u1 = FrameStep * IWidth + IWidth, v1 = IHeight;
			CP_Image_DrawSubImage(SImg, m->x, m->y,m->w,m->h, u0, v0, u1, v1, alpha);
			break;
		default:
			break;
	}
}


/*
@brief		Handles Collision between Mobs and Player
@params		Mob	-> current mob obj
			Player	-> target player location
@returns	Nothing
*/

/*
@brief		Function that check mob to mob collision
@params		mob	-> Pointer to Mob
			P	-> Pointer to Player
			tracker	-> Pointer to WaveTrack[No_Waves] (in map.c)
@return		
*/
float square(float one, float two) {
	 return one * one + two * two;
}
void MobTMobCollision(Mob* m, Player* p, WaveTrack* tracker, int const No_Waves) {
	if (m->h > 0) {
		int status = 0;
		Mob* tm, * main, * bounce;
		/*
			3 Objs m, tm, p;
			Vector, distance from m to p
			Distance from m to tm
			Distance from tm to p

			radius of contact of m && tm
			dist will be left in squared form
		*/
		float mRad = square(m->w/2, m->h/2), tmRad;
		float dMtoP = square(p->x-m->x, p->y-m->y), dTMtoP, dMtoTM;
		CP_Vector vMtoP = CP_Vector_Set(p->x - m->x, p->y - m->y);
		CP_Vector NormBase = CP_Vector_Normalize(vMtoP);
		CP_Vector BasePF = CP_Vector_Scale(NormBase, m->CStats.Speed);
		goto BasicMovement;
		if (CP_System_GetFrameRate() < 27.0f) {
			goto BasicMovement;
		}
		//Some complicated ass iter using pointers XD
		for (WaveTrack* i = tracker; i < tracker + No_Waves; i++) {
			if (i->CurrentCount != 0) {
				for (Mob** j = i->arr; j < i->arr + i->MobCount; j++) {
					if (m == *j) {
						continue;
					}
					tm = *j;
					if (tm->Status == 1) {
						/*
						Collision:
							->if M collide with another mob and is same vector direction pause
						*/
						tmRad = square(tm->w / 2, tm->h / 2);
						dMtoTM = square(m->x - tm->x, m->y - tm->y);
						dTMtoP = square(p->x - tm->x, p->y - tm->y);
						if (dMtoTM <= mRad + tmRad && dMtoP < dTMtoP) {
							main = m;
							bounce = tm;
							/*
							Require:
								Vector bounce to main
								Vector bounce to p
							*/
							CP_Vector vBounceToMain = CP_Vector_Set(m->x - tm->x, m->y - tm->y);
							CP_Vector vBounceToP = CP_Vector_Set(p->x - tm->x, p->y - tm->y);
							if (vBounceToMain.x == vBounceToP.x && vBounceToMain.y == vBounceToMain.y) {
								goto BasicMovement;
							}
							float BouncePAngle = CP_Vector_Angle(vBounceToMain, vBounceToP);
							if (_isnanf(BouncePAngle)) {
								goto BasicMovement;
							}
							float nAngle = CP_Random_RangeFloat(0, BouncePAngle);
							//Using vBounceToP as main directional vector -> find new angle from it -> transform it -> move bounce in reverse direction
							CP_Matrix rot = CP_Matrix_Set(
								cos(nAngle), -sin(nAngle), 0,
								sin(nAngle), cos(nAngle), 0,
								0, 0, 0
							);
							CP_Vector nDirection = CP_Vector_Normalize(CP_Vector_MatrixMultiply(rot, vBounceToP));
							CP_Vector mainDirection = CP_Vector_Scale(CP_Vector_Normalize(CP_Vector_Set(p->x - m->x, p->y - m->y)), m->CStats.Speed);
							bounce->x -= nDirection.x;
							bounce->y -= nDirection.y;
							if (m != main) {
								main->x += mainDirection.x;
								main->y += mainDirection.y;
								status = 1;
							}
							goto BasicMovement;
						}
					}
				}
			}
		}
	BasicMovement:
		if (0 == status) {
			m->x += BasePF.x;
			m->y += BasePF.y;
		}
	}
}

void MobTPlayerCollision(Mob* m, Player* p) {
	
	if (CP_Vector_Length(CP_Vector_Set(p->x-m->x, p->y-m->y)) <= p->HITBOX * 2) {
		m->CStats.HP -= p->STATTOTAL.DAMAGE_TOTAL;
		//p->CURRENT_HP -= m->CStats.Dmg;
	}
	if (m->CStats.HP <= 0) {
		m->Status = 0;
	}
}

 


void PrintWaveStats(int* CWaveCount,int NO_WAVES, int* WaveIDQueue, int* MobCount) {

	//Result Print Start
	printf("\nCurrent Wave: %d\nWave Queue: ", *CWaveCount);
	for (int i = 0; i < NO_WAVES; i++) {
		printf("| %d ", WaveIDQueue[i]);
	}
	printf("\nMob Count: ");
	int tMob = 0;
	for (int i = 0; i < NO_WAVES; i++) {
		printf("| %d ", MobCount[i]);
		tMob += MobCount[i];
	}
	printf(" |Total: %d\n", tMob);
	//Result Print End
}

void FreeMobResource(WaveTrack* wtracker,int noWaves, CP_Image* spritesheet, int Mob_Img) {
	for (int i = 0; i < noWaves; i++) {
		for (int a = 0; a < wtracker[i].arrSize; a++) {
			free(wtracker[i].arr[a]);
		}
		free(wtracker[i].arr);
	}
	for (int i = 0; i < Mob_Img; i++) {
		CP_Image* c = spritesheet[i];
		CP_Image_Free(&(spritesheet[i]));
		free(spritesheet[i]);
	}

	//free(spritesheet);
}
//
//
//

/*
//			Old Code(Scraped)
//			  Ignore Below
//*/
//					//tmRad = (pow(tm->w / 2, 2) + pow(tm->h / 2, 2));
//					//dMtoTM = (pow(m->x - tm->x, 2) + pow(m->y - tm->y, 2));
//					//dTMtoP = (pow(p->x - tm->x, 2) + pow(p->y - tm->y, 2));
//					//if (dMtoTM <= mRad + tmRad) {
//					//	goto BasicMovement;
//					//}
//					//if (dMtoP < dTMtoP ) {
//					//	main = m;
//					//	bounce = tm;
//					//	/*
//					//	Require:
//					//		Vector bounce to main
//					//		Vector bounce to p
//					//	*/
//					//	CP_Vector vBounceToMain = CP_Vector_Set(m->x - tm->x, m->y - tm->y);
//					//	CP_Vector vBounceToP = CP_Vector_Set(p->x - tm->x, p->y - tm->y);
//					//	if (vBounceToMain.x == vBounceToP.x && vBounceToMain.y == vBounceToMain.y) {
//					//		goto BasicMovement;
//					//	}
//					//	float BouncePAngle = CP_Vector_Angle(vBounceToMain, vBounceToP);
//					//	if (_isnanf(BouncePAngle)) {
//					//		goto BasicMovement;
//					//	}
//					//	float nAngle = CP_Random_RangeFloat(0, BouncePAngle);
//					//	//Using vBounceToP as main directional vector -> find new angle from it -> transform it -> move bounce in reverse direction
//					//	CP_Matrix rot = CP_Matrix_Set(
//					//		cos(nAngle), -sin(nAngle), 0,
//					//		sin(nAngle), cos(nAngle), 0,
//					//		0, 0, 0
//					//	);
//					//	CP_Vector nDirection = CP_Vector_Normalize(CP_Vector_MatrixMultiply(rot, vBounceToP));
//					//	CP_Vector mainDirection = CP_Vector_Scale(CP_Vector_Normalize(CP_Vector_Set(p->x - m->x, p->y - m->y)), m->CStats.Speed);
//					//	bounce->x -= nDirection.x;
//					//	bounce->y -= nDirection.y;
//					//	if (m != main) {
//					//		main->x += mainDirection.x;
//					//		main->y += mainDirection.y;
//					//		status = 1;
//					//	}
//					//	goto BasicMovement;
