#pragma once
#include "cprocessing.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Mob.h"
#include "Map.h"





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
			cStat->Speed = 10;
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
void CreateMob(Mob*m, MobStats *Base, Player*player, int offSet)
{
	float MaxRadius = (float) WHeight * BoundScale;
	//Uncomment below if you want to manually set spawn radius
	//MaxRadius = 600;
	//Assume Player center of spawnable area
	float nx, ny, rTheta, r, Diff = 0.0;
	//do {
	//} while (Diff <= offSet);
		/*Formula for generating points in circle*/
	rTheta = CP_Random_RangeFloat(0, 1) * 2 * PI;
	//eqn = sqrt( random() * (MaxRadius**2 - MinRadius**2) + MinRadius**2 ) <- MinRadius == offSet
	r = sqrt(CP_Random_RangeFloat(0, 1) * (squareDist(MaxRadius,0) - squareDist(offSet, 0)) + squareDist(offSet,0));
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





int WaveIDQueue[NO_WAVES], CWave, MaxMob, CWaveCost;
int MobCount[NO_WAVES];
WaveTrack WaveTracker[NO_WAVES];
void CreateWaveTracker(void) {
	CWave = 0, MaxMob = MaxMobGrowthRate, CWaveCost = WaveCostGrowthRate;
	for (int i = 0; i < NO_WAVES; i++) {
		WaveTracker[i] = (WaveTrack){
			MaxMob, //Max Mob
			0, //MobCount
			0, //Current Mob Count
			0, //Wave Cost
			StartMobQuantity, //Array Size 
			SpawnAreaOffset, //Spawn offset
			malloc(sizeof(Mob*) * StartMobQuantity) //, //Arr
		};
	//		white //Wave Color
	//	};
		InitWavesArr(&WaveTracker[i], 0);
		WaveIDQueue[i] = -1;
		MobCount[i] = 0;
	}
}

void InitWavesArr(WaveTrack* tracker, int start) {

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

void GenerateWaves(void) {
	int cSec = (int)CP_System_GetSeconds();
	if (cSec % Wave_Timer == 0)
		MaxMob += MaxMobGrowthRate;
	if (cSec % Spawn_Timer == 0) {
		CWaveCost += WaveCostGrowthRate;
		for (int i = 0; i < NO_WAVES; i++) 
		{
			//At default WaveIDQueue = {-1,-1,-1,-1}
			//Whereby each "-1" == to available slot to generate waves
			if (WaveIDQueue[i] == -1) {
				CWave += 1; //Increment WaveCount
				if (MaxMob >= (MaxUpperLimit / NO_WAVES)) {
					WaveTracker[i].MaxMob = (MaxUpperLimit / NO_WAVES);
				}
				else {
					WaveTracker[i].MaxMob = MaxMob;//Update Max Mob limit
					WaveTracker[i].WaveCost = CWaveCost;	//Update Value which allows spawning of mobs
				}
				//Generate Waves at avaiable slot 
				GenerateMobs(&WaveTracker[i], &P);
				//printf("\n\tCreated Wave: %d\n", *WaveCount);
				//Edit increment to spawn more mob each waves
				WaveIDQueue[i] = CWave; //Update waves of queue at [i]
				MobCount[i] = WaveTracker[i].MobCount;
				break;
			}
		}	
	}

}



/*
@brief		Function To load images
@params		Sprites	-> array that contains sprites for mobs
			Size	-> Size of array
@returns	Filled up array of pointers to CP_Image Objs
*/
CP_Image** MobSprites;
int Mob_Img;
void MobLoadImage(void) {
	char *FilePaths[] = {
		"./Assets/Mobs/SmallM.png",
		"./Assets/Mobs/SmallM_Flipped.png",
		"./Assets/Mobs/RangeM.png",
		"./Assets/Mobs/RangeM_Flipped.png"
	};
	/*
	(Walk = +0, Flipped = +1)
	Small Mob  => 0 ~ 1
	Medium Mob => 2 ~ 3
	Big Mob    => 4 ~ 5
	*/

	Mob_Img = (sizeof(FilePaths) / sizeof(FilePaths[0]));
	MobSprites = malloc(sizeof(CP_Image*) * Mob_Img);
	for (int i = 0; i < Mob_Img; i++) {
		MobSprites[i] = malloc(sizeof(CP_Image));
		MobSprites[i] = CP_Image_Load(FilePaths[i]);
	}

}
void DrawMobImage(Mob* m, Player* p) {

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
	CP_Image* SImg = MobSprites[StartImgI];
	IHeight = CP_Image_GetHeight(SImg);

	switch (m->Title) {
		case SmallMob:
			SizeDef = 5, targetFPS = 6;
			IWidth = CP_Image_GetWidth(SImg) / SizeDef;
			m->h = IHeight * Img_Scale / IHeight, m->w = m->h;
			FrameStep = (m->AnimationCycle / targetFPS) % SizeDef;
			u0 = FrameStep * IWidth;
			v0 = 0;
			u1 = FrameStep * IWidth + IWidth;
			v1 = IHeight;


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
float squareDist(float one, float two) {
	if (two == 0)
		return one * one;
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
		float mRad = squareDist(m->w/2, m->h/2), tmRad;
		float dMtoP = squareDist(p->x-m->x, p->y-m->y), dTMtoP, dMtoTM;
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
						tmRad = squareDist(tm->w / 2, tm->h / 2);
						dMtoTM = squareDist(m->x - tm->x, m->y - tm->y);
						dTMtoP = squareDist(p->x - tm->x, p->y - tm->y);
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
		p->CURRENT_HP -= m->CStats.Dmg;
	}
	if (m->CStats.HP <= 0) {
		m->Status = 0;
	}
}

 


void PrintWaveStats(void) {

	//Result Print Start
	printf("\nCurrent Wave: %d\nWave Queue: ", CWave);
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

void FreeMobResource() {
	for (int i = 0; i < NO_WAVES; i++) {
		for (int a = 0; a < WaveTracker[i].arrSize; a++) {
			free(WaveTracker[i].arr[a]);
		}
		free(WaveTracker[i].arr);
	}
	for (int i = 0; i < Mob_Img; i++) {
		CP_Image* c = *MobSprites[i];
		CP_Image_Free(&(MobSprites[i]));
		free(MobSprites[i]);
	}
	free(MobSprites);

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
