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
			cStat->DEF = 10 + statscale;
			cStat->Speed = 8 + statscale;
			cStat->Range = 0 + statscale;
			cStat->Dmg = 1 + statscale;
			cStat->size = 50;
			break;
		case MediumMob:
			cStat->HP = 10 * statscale * 2;
			cStat->DEF = 10;
			cStat->Speed = 5 + statscale;
			cStat->Range = 0 + statscale;
			cStat->Dmg = 2 + statscale;
			cStat->size = 60 + statscale;
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
	m->coor = CP_Vector_Add(CP_Vector_Scale(CP_Vector_Set(cos(rTheta), sin(rTheta)), r), player->coor);

	m->BaseStats = *Base;
	m->CStats = *Base;
	//m->x = nx;
	//m->y = ny;
	m->Status = 1;
	m->AnimationCycle = 0;
	m->w = 0;
	m->h = 0;
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

	MobStats bs = (MobStats){
		0,0,0,0,0,0
	};
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
	//MobNode* root = tracker->tree, *current;
	
	while (cost > 0) {
		if (MobC == tracker->MaxMob) {
			break;
		}
		randM = CP_Random_RangeInt(0, 1);
		//randM = 0;
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
	if (MobCycleTimer != (int)CP_System_GetSeconds()) {
		MobCycleTimer = (int)CP_System_GetSeconds() - MobCycleTimer > 1 ? (int)CP_System_GetSeconds() : MobCycleTimer + 1;
		printf("Mob Cycle Timer: %d | Stat Scale: %f\n",MobCycleTimer, statscale);
		//printf("Current FPS: %f\n", CP_System_GetFrameRate());
		if (MobCycleTimer % Wave_Timer == 0) {
			statscale *= 1.1;
			MaxMob += MaxMobGrowthRate;
		}
		if (MobCycleTimer % Spawn_Timer == 0) {
			for (int i = 0; i < NO_WAVES; i++)
			{
				//At default WaveIDQueue = {-1,-1,-1,-1}
				//Whereby each "-1" == to available slot to generate waves
				if (WaveIDQueue[i] == -1) {
					++CWave; //Increment WaveCount
					if (MaxMob >= (MaxUpperLimit / NO_WAVES)) {
						WaveTracker[i].MaxMob = (MaxUpperLimit / NO_WAVES);
						WaveTracker[i].WaveCost = CWaveCost;
					}
					else {
						WaveTracker[i].MaxMob = MaxMob;			//Update Max Mob limit
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
			//PrintWaveStats();
			//printf("\t\tTimer: %d", MobCycleTimer);
		}
	}
//	printf("%f\n", CP_System_GetSeconds());
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
	
	int IHeight, IWidth, alpha = 255,original_Size, scale, leftOS = 0, rightOS = 0, topOS = 0, btmOS = 0;
	int SizeDef = 5, StartImgI = m->Title * 2, flip = 0;
	int FrameStep = 0, targetFPS = 6;
	m->AnimationCycle += 1;
	int u0 = 0, v0 = 0, u1 = 0, v1 = 0;
	/*
	SM = 0, 0 * 2 = 0
	MM = 1, 1 * 2 = 2
	BM = 2, 2 * 2 = 4
	*/
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
void MobTMobCollision(Mob* m) {
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
		float dMtoP = squareDist(P.x-m->coor.x, P.y-m->coor.y), dTMtoP, dMtoTM;
		CP_Vector vMtoP = CP_Vector_Subtract(P.coor, m->coor);
		CP_Vector NormBase = CP_Vector_Normalize(vMtoP);
		CP_Vector BasePF = CP_Vector_Scale(NormBase, m->CStats.Speed);
#pragma region
		goto BasicMovement;
		if (CP_System_GetFrameRate() < 27.0f) {
			goto BasicMovement;
		}
		//Some complicated ass iter using pointers XD
		for (WaveTrack* i = WaveTracker; i < WaveTracker + NO_WAVES; i++) {
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
						dMtoTM = squareDist(m->coor.x - tm->coor.x, m->coor.y - tm->coor.y);
						dTMtoP = squareDist(P.x - tm->coor.x, P.y - tm->coor.y);
						if (dMtoTM <= mRad + tmRad && dMtoP < dTMtoP) {
							main = m;
							bounce = tm;
							/*
							Require:
								Vector bounce to main
								Vector bounce to p
							*/
							CP_Vector vBounceToMain = CP_Vector_Set(m->coor.x - tm->coor.x, m->coor.y - tm->coor.y);
							CP_Vector vBounceToP = CP_Vector_Set(P.x - tm->coor.x, P.y - tm->coor.y);
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
							CP_Vector mainDirection = CP_Vector_Scale(CP_Vector_Normalize(CP_Vector_Set(P.x - m->coor.x, P.y - m->coor.y)), m->CStats.Speed);
							//bounce->x -= nDirection.x;
							//bounce->y -= nDirection.y;
							bounce->coor = CP_Vector_Subtract(bounce->coor, nDirection);
							if (m != main) {
								main->coor = CP_Vector_Add(main->coor, mainDirection);
							//	main->x += mainDirection.x;
							//	main->y += mainDirection.y;
								status = 1;
							}
							goto BasicMovement;
						}
					}
				}
			}
		}

#pragma endregion
	BasicMovement:
		if (0 == status) {
			m->coor = CP_Vector_Add(m->coor, BasePF);
		//	m->x += BasePF.x;
		//	m->y += BasePF.y;
		}
	}
}

void MobTPlayerCollision(Mob* m, Player* p) {
	
	if (CP_Vector_Length(CP_Vector_Subtract(m->coor, p->coor)) <= p->HITBOX) {
		m->CStats.HP -= p->STATTOTAL.DAMAGE_TOTAL;
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