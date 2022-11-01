#include "cprocessing.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "player.h"
#include "Mob.h"

//#include "player.h"

#define MOB_CAP 500
#define Img_Scale 80


//Pool of Mob Types
//Edit here for different Mob Types
/*
@brief		Function to update mob's base stats based off type given
@params		cStat	-> current mob's base stats
			type	-> types of values to update with.
@returns	Updated base stats with new values
*/
int MobCosts[MobTypes] = { 1,2,3, 4, 5};
void CreateBaseStat(MobStats* cStat, int type) {
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
void CreateMob(Mob*m, MobStats *Base, Player*player, int offSet)
{
	float BoundScale = 0.5f, MaxRadius = CP_Math_Distance(0, 0, CP_System_GetWindowWidth(), CP_System_GetWindowHeight())* BoundScale;
	//Uncomment below if you want to manually set spawn radius
	//MaxRadius = 600;
	//Assume Player center of spawnable area
	float nx, ny, rTheta, r, Diff = 0.0;
	do {
		/*Formula for generating points in circle*/
		rTheta = CP_Random_RangeFloat(0, 1) * 2 * PI;
		//eqn = sqrt( random() * (MaxRadius**2 - MinRadius**2) + MinRadius**2 ) <- MinRadius == offSet
		r = sqrt(CP_Random_RangeFloat(0, 1) * (pow(MaxRadius, 2) - pow(offSet, 2)) + pow(offSet, 2));
		nx = player->x + r * cos(rTheta);
		ny = player->y + r * sin(rTheta);
		Diff = CP_Math_Distance(nx, ny, player->x, player->y);
	} while (Diff <= offSet);

	m->BaseStats = *Base;
	m->CStats = *Base;
	m->x = nx;
	m->y = ny;
	m->Status = 1;
	m->AnimationCycle = 0;
	m->w = 0;
	m->h = 0;
}

/*
@brief		Initialise Mob arrays with Blank Mobs with blank MobStats
@params		WaveTrack struct pointer to be filled with blank mobs
@returns	WaveTrack's array to be filled with blank mobs (to be modified/updated when needed)
*/
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
		tracker->arr[w]->Status = 0;
		tracker->arr[w]->AnimationCycle = 0;
		tracker->arr[w]->w = 0;
		tracker->arr[w]->h = 0;
	}
}

//Function that generated Mobs based off a cost system
/*
@brief		Generates Mobs, Fills up a single array with mobs
@params		tracker ->	a struct containing wave details, 
						wave array for mobs and 
						spawn offset area around player
			player	->	contains player's coordinates
@returns	Fills in a array of Mob pointers pointing to blank mob objects (created in InitWaveArr())
*/
void GenerateMobs(WaveTrack* tracker, Player* player) {
	int MobC = 0, cost = tracker->WaveCost, randM, randMCost;

	while (cost > 0) {
		//Generate N no of mobs based off wave cost
		//randM = CP_Random_RangeInt(0, MobTypes - 1);
		randM = CP_Random_RangeInt(0,1);
		//randM = 0;
		randMCost = MobCosts[randM];

		//Expand array
		if (MobC >= tracker->arrSize) {
			int nQ = tracker->arrSize * 2;
			Mob** tArr = realloc(tracker->arr,sizeof(Mob*) * nQ);
			if (NULL != tArr) {
				tracker->arr = tArr;
				tracker->arrSize = nQ;
			}
			InitWavesArr(tracker, MobC);
		}

		if (MobC == tracker->MaxMob) {
			//printf("\tMax Mob Limit\n");
			break;
		}
		if (cost >= randMCost) {
			Mob* cMob = tracker->arr[MobC];
			cMob->Title = randM;
			CreateBaseStat(&cMob->BaseStats, randM);
			CreateMob(cMob, &cMob->BaseStats, player, tracker->spawnOffset);
			cost -= randMCost;
			MobC += 1;

		}
	}

	//for (int i = 0; i < MobC; i++) {
	//	printf("%d (%f, %f)\n", tracker->arr[i]->Title, tracker->arr[i]->x, tracker->arr[i]->y);
	//}
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
			queue[i].MaxMob = MaxMobGrowth; //Update Max Mob limit
			queue[i].WaveCost = CostGrowth;	//Update Value which allows spawning of mobs
			//Generate Waves at avaiable slot 
			GenerateMobs(&queue[i], P);
			printf("\n\tCreated Wave: %d\n", *WaveCount);
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
@brief		Algo to determine how each mobs move
@params		mob	-> target mob to update movement
			tX	-> target Position on x axis
			tY	-> target Position on y axis
@returns	Update Mob with new coordinates values
*/
void MobPathFinding(Mob* mob, float tX, float tY) {
	CP_Vector v;
	switch (mob->Title) {
	default:
		v = CP_Vector_Normalize(CP_Vector_Set(mob->x - tX, mob->y - tY));
		mob->x -= v.x * mob->CStats.Speed;
		mob->y -= v.y * mob->CStats.Speed;
		break;
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
void DrawMobImage(CP_Image* Sprites, Mob*m, Player *p) {

	int IHeight, IWidth, alpha = 255;
	int SizeDef, StartImgI = m->Title * 2, Step, targetFPS = 6;
	m->AnimationCycle += 1;
	int u0, v0, u1, v1;
		/*
		u0 (float) - The left most pixel of the sub-image(far left = 0).
		v0 (float) - The top most pixel of the sub-image (top = 0).
		u1 (float) - The right most pixel for the sub-image (far right = image width).
		v1 (float) - The bottom most pixel for the sub-image (bottom = image height).
		*/
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
		SizeDef = 5, targetFPS = 6, IWidth = CP_Image_GetWidth(SImg) / SizeDef;
		m->h = IHeight * Img_Scale / IHeight, m->w = m->h;

		Step = (m->AnimationCycle / targetFPS) % SizeDef;
		u0 = Step * IWidth, v0 = 0, u1 = Step * IWidth + IWidth, v1 = IHeight;

		CP_Image_DrawSubImage(SImg, m->x, m->y,m->w, m->h, u0, v0, u1, v1, alpha);
		break;
	case MediumMob:
		SizeDef = 2,targetFPS = 6, IWidth = CP_Image_GetWidth(SImg) / SizeDef;
		m->h = IHeight * Img_Scale / IHeight, m->w = m->h;
		
		Step = ( m->AnimationCycle / targetFPS) % SizeDef;
		u0 = Step * IWidth, v0 = 0, u1 = Step * IWidth + IWidth, v1 = IHeight;
	
		CP_Image_DrawSubImage(SImg, m->x, m->y,m->w,m->h, u0, v0, u1, v1, alpha);
		break;
	default:
		break;
	}
	//CP_Graphics_DrawCircle(m->x, m->y, CP_Vector_Distance(CP_Vector_Zero(), CP_Vector_Set(m->w / 2, m->h / 2)));
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
void MobTMobCollision(Mob* m, Player* p, WaveTrack* tracker, int No_Waves) {
	if (m->h > 0) {
		int Gap, status = 0;
		//m's radius equivalent (squared version)
		Mob* tm, * main, * bounce;
		/*
			3 Objs m, tm, p;
			Vector, distance from m to p
			Distance from m to tm
			Distance from tm to p

			radius of contact of m && tm
			dist will be left in squared form
		*/
		float mRad = (pow(m->w / 2, 2) + pow(m->h / 2, 2)), tmRad;
		float dMtoP = (pow(p->x - m->x, 2) + pow(p->y - m->y, 2)), dMtoTM, dTMtoP;
		CP_Vector vMtoP = CP_Vector_Set(p->x - m->x, p->y - m->y);
		CP_Vector BasePF = CP_Vector_Scale(CP_Vector_Normalize(vMtoP), m->CStats.Speed);
		for (int i = 0; i < No_Waves; i++) {
			for (int j = 0; j < tracker[i].MobCount; j++) {
				if (m == tracker[i].arr[j]) {
					continue;
				}
			
				tm = tracker[i].arr[j];
				if (tm->Status == 0) {
					continue;
				}
				/*
				Collision Algo
					: Iterate thru all other mobs
					: If collide, reverse movement
				*/
				//Circle Collision
				/*	Filter with radius first
						: Width & Height defined with Img_Scale
						: Rad = sqrt( Img_Scale / 2 **2 + Img_Scale / 2 ** 2)
						: Diff Img Might have diff width -> Compare the squared version
				*/
				//Can dabble with separating axis theorem
				//Bounce mechanics, priority = closer mob to player will proceed towards player, other will bounce random direction
				if (tm->h > 0) {
					tmRad = (pow(tm->w / 2, 2) + pow(tm->h / 2, 2));
					dMtoTM = (pow(m->x - tm->x, 2) + pow(m->y - tm->y, 2));
					if (dMtoTM <= mRad + tmRad) {
						dTMtoP = (pow(p->x - tm->x, 2) + pow(p->y - tm->y, 2));
						main = m;
						bounce = tm;
						if (dMtoP > dTMtoP) {
							main = tm;
							bounce = m;
						}
						/*
						Require:
							Vector bounce to main
							Vector bounce to p
						*/
						CP_Vector vBounceToMain = CP_Vector_Set(main->x - bounce->x, main->y - bounce->y);
						CP_Vector vBounceToP = CP_Vector_Set(p->x - bounce->x, p->y - bounce->y);
						float BouncePAngle = CP_Vector_Angle(vBounceToMain, vBounceToP);
						if (_isnanf(BouncePAngle)) {
							break;
						}
						float nAngle = CP_Random_RangeFloat(0, BouncePAngle);
						//Using vBounceToP as main directional vector -> find new angle from it -> transform it -> move bounce in reverse direction
						CP_Matrix rot = CP_Matrix_Set(
							cos(nAngle), -sin(nAngle), 0,
							sin(nAngle), cos(nAngle), 0,
							0, 0, 0
						);
						CP_Vector nDirection = CP_Vector_Scale(CP_Vector_Normalize(CP_Vector_MatrixMultiply(rot, vBounceToP)), bounce->CStats.Speed);
						CP_Vector mainDirection = CP_Vector_Scale(CP_Vector_Normalize(CP_Vector_Set(p->x - main->x, p->y - main->y)), main->CStats.Speed);
						bounce->x -= nDirection.x;
						bounce->y -= nDirection.y;
						if (m != main) {
							main->x += mainDirection.x;
							main->y += mainDirection.y;
						}
						break;

					}
				}
			}
			m->x += BasePF.x;
			m->y += BasePF.y;
		}
	}
}

void MobTPlayerCollision(Mob* m, Player* p) {
		if (CP_Vector_Length(CP_Vector_Set(p->x-m->x, p->y-m->y)) <= p->HITBOX) {
			m->CStats.HP -= p->DAMAGE;
			p->CURRENT_HP -= m->CStats.Dmg;
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


/*
			Old Code(Scraped)
			  Ignore Below
*/
//void GenerateWaves(WaveTrack *tracker, Player*player) {
//	//gMobCount = Generated Mob Count throughout this func
//	//waveCost = Amt of "currency" the func will take to generated random types of mobs per wave
//	int gMobCount = 0, waveCost = tracker->waveCost;
//	int Reused = 0;
//	if (tracker->arr == NULL) {
//		printf("Here\n");
//	}
//
//	//int xLeft = 0, xRight = CP_System_GetWindowWidth(), yTop = 0, yBtm = CP_System_GetWindowHeight();
//	while (waveCost > 0) {
//		//Generate a random mob from mob types pool
//		//Edit here to generated different Mob Types
//		int randMobI = CP_Random_RangeInt(0, 2);
//		int randMobCost = MobCosts[randMobI];
//		//Algo to extend array size when nearing max capacity
//		//Provides array with overhead for additional 100 Mobs.
//		if (gMobCount >= tracker->arrSize) {
//			int nQuantity = tracker->arrSize + 1000;
//			
//			Mob *temp = realloc(tracker->arr, sizeof(Mob) * nQuantity);
//			if (temp != NULL) {
//				//IF realloc is successful
//				//Update tracker with new parameters
//				//free(tracker->arr);
//				tracker->arr = temp;
//				tracker->arrSize = nQuantity;
//			}
//			else {
//				Mob* nArr = malloc(sizeof(Mob) * nQuantity);
//				for (int k = 0; k < tracker->arrSize; k++) {
//					nArr[k] = tracker->arr[k];
//				}
//				free(tracker->arr);
//				tracker->arr = nArr;
//			}
//			free(temp);
//			printf("\n\tArray Expended");
//			printf("\n\tNew Array Size: %d\n", tracker->arrSize);
//			continue;
//		}
//		
//		//Algo to add Mob generated to array.
//		if (waveCost >= randMobCost) {
//			if (tracker->MobCount > 0 && tracker->arr[gMobCount].Status == 0) {
//				CreateBaseStat(&tracker->arr[gMobCount].BaseStats, randMobI);
//				CreateMob(&tracker->arr[gMobCount], randMobI, &tracker->arr[gMobCount].BaseStats, player, tracker->spawnOffset);
//				Reused += 1;
//
//			}
//			else {
//				Mob m;
//				MobStats ms;
//				CreateBaseStat(&ms, randMobI);
//				CreateMob(&m, randMobI, &ms, player, tracker->spawnOffset);
//				tracker->arr[gMobCount] = m;
//			}
//			//Generated a new mob at specified locations
//			//printf("Pos: %d -> Title: %d | X: %d | Y: %d\n",gMobCount,m.Title,(int) m.coor.x,(int) m.coor.y);
//			//printf("%p\n", &m);
//			
//			gMobCount += 1;
//			waveCost -= randMobCost;
//		}
//	}
//	tracker->MobCount = gMobCount;
//	tracker->CurrentCount = gMobCount;
//	printf("Mobs Resused: %d\n", Reused);
//	printf("New Mobs Created: %d\n", gMobCount - Reused);
//}

//Depreceated 
/*
@brief		Function to draw mob
@params		Mob	-> pointer to target mob
			r, g, b	-> RGB values for CP_Color_Create //will change to image/sprite
@returns	Nothing
*/
//void DrawMob(Mob* mob, int r, int g, int b)
//{
//	CP_Settings_StrokeWeight(0.5f);
//	int alpha = (mob->CStats.HP / mob->BaseStats.HP) * 255;
//	CP_Settings_Fill(CP_Color_Create(r,g,b, alpha));
//	CP_Graphics_DrawCircle((double) mob->x, (double) mob->y, mob->CStats.size);
//}
