#include "cprocessing.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "player.h"
#include "Mob.h"

//#include "player.h"

#define Img_Scale 80
#define BoundScale 1.2f



// A utility function to get the height of the tree
int treeheight(MobNode *N)
{
    if (N == NULL)
        return 0;
	return 1+max(treeheight(N->left), treeheight(N->right));
}

// A utility function to get maximum of two integers
//int max(int a, int b);
 MobNode* newNode(Mob mob)
{
    MobNode* node = malloc(sizeof(MobNode));
	node->key = mob;
    node->left   = NULL;
    node->right  = NULL;
    node->h = 0;  // new node is initially added at leaf
    return(node);
}

/* Helper function that allocates a new node with the given key and
    NULL left and right pointers. */
 
// A utility function to right rotate subtree rooted with y
// See the diagram given above.
MobNode *rightRotate(MobNode *y)
{
    MobNode *x = y->left;
    MobNode *T2 = x->right;
 
    // Perform rotation
    x->right = y;
    y->left = T2;
 
    // Update heights
    y->h = treeheight(y);
    x->h = treeheight(x);
 
    // Return new root
    return x;
}
 
// A utility function to left rotate subtree rooted with x
// See the diagram given above.
MobNode *leftRotate(MobNode *x)
{
    MobNode *y = x->right;
    MobNode *T2 = y->left;
 
    // Perform rotation
    y->left = x;
    x->right = T2;
 
    //  Update heights
    x->h = treeheight(x);
    y->h = treeheight(y);
 
    // Return new root
    return y;
}
 
// Get Balance factor of node N
int getBalance(MobNode *N)
{
    if (N == NULL)
        return 0;
    return treeheight(N->left) - treeheight(N->right);
}
float getX(MobNode* N) {
    return N->key.x;
}
// Recursive function to insert a key in the subtree rooted
// with node and returns the new root of the subtree.
MobNode* insert(MobNode* node, Mob key)
{
	/* 1.  Perform the normal BST insertion */
	if (node == NULL)
		return(newNode(key));

	if (key.x < node->key.x)
		node->left = insert(node->left, key);
	else if (key.x > node->key.x)
		node->right = insert(node->right, key);
	else // Equal keys are not allowed in BST
		return NULL;
	//}
    /* 2. Update height of this ancestor node */
    node->h = treeheight(node);
 
    /* 3. Get the balance factor of this ancestor
          node to check whether this node became
          unbalanced */
    int balance = getBalance(node);
 
    // If this node becomes unbalanced, then
    // there are 4 cases
 
    // Left Left Case
    //if (balance > 1 && key.x < node->left->key->x)
    if (balance > 1 && key.x < getX(node->left))
        return rightRotate(node);
 
    // Right Right Case
    if (balance < -1 && key.x > getX(node->right))
        return leftRotate(node);
 
    // Left Right Case
    if (balance > 1 && key.x > getX(node->left))
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }
 
    // Right Left Case
	if (balance < -1 && key.x < getX(node->right))
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}
    /* return the (unchanged) node pointer */
    return node;
}


void freeTree(MobNode* node) {
	if (NULL == node) {
		return;
	}
	freeTree(node->left);
	freeTree(node->right);
	free(node);
}

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
void CreateMob(Mob*m, MobStats *Base, Player*player, int offSet)
{
	float MaxRadius = CP_Vector_Length(CP_Vector_Zero(), CP_Vector_Set(CP_System_GetWindowWidth()/2, CP_System_GetWindowHeight()/2)) * BoundScale;
	//Uncomment below if you want to manually set spawn radius
	//MaxRadius = 600;
	//Assume Player center of spawnable area
	float nx, ny, rTheta, r, Diff = 0.0;
	//do {
	//} while (Diff <= offSet);
		/*Formula for generating points in circle*/
	rTheta = CP_Random_RangeFloat(0, 1) * 2 * PI;
	//eqn = sqrt( random() * (MaxRadius**2 - MinRadius**2) + MinRadius**2 ) <- MinRadius == offSet
	r = sqrt(CP_Random_RangeFloat(0, 1) * (pow(MaxRadius, 2) - pow(offSet, 2)) + pow(offSet, 2));
	nx = player->x + r * cos(rTheta);
	ny = player->y + r * sin(rTheta);
	Diff = CP_Math_Distance(nx, ny, player->x, player->y);

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

void traverseTree(CP_Image *Sprites, MobNode *N, Player*p, WaveTrack *tracker) {
	if (NULL == N) {
			return;
	}
	traverseTree(Sprites, N->left, p, tracker);
	traverseTree(Sprites, N->right,p, tracker);
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
	for (int i = 0; i < MobC; i++) {
		tracker->tree = insert(tracker->tree, *tracker->arr[i]);
	}

	//free(tarr);

//	while (cost > 0) {
//		if (MobC == tracker->MaxMob) {
//			//printf("\tMax Mob Limit\n");
//			break;
//		}
//		//Generate N no of mobs based off wave cost
//		//randM = CP_Random_RangeInt(0, MobTypes - 1);
//		randM = CP_Random_RangeInt(0,1);
//		//randM = 0;
//		randMCost = MobCosts[randM];
//
//		//Expand array
//		if (MobC >= tracker->arrSize) {
//			int nQ = tracker->arrSize * 2;
//			Mob** tArr = realloc(tracker->arr,sizeof(Mob*) * nQ);
//			if (NULL != tArr) {
//				tracker->arr = tArr;
//				tracker->arrSize = nQ;
//			}
//			InitWavesArr(tracker, MobC);
//			printf("\tNew Arr Size = %d", nQ);
//		}
//
//		if (cost >= randMCost) {
//			Mob* cMob = tracker->arr[MobC];
//			cMob->Title = randM;
//			CreateBaseStat(&cMob->BaseStats, randM);
//			CreateMob(cMob, &cMob->BaseStats, player, tracker->spawnOffset);
//			cost -= randMCost;
//			MobC += 1;
//
//		}
//	}

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
			if (queue[i].tree != NULL) {
				freeTree(queue[i].tree);
				queue[i].tree = NULL;
			}
			*WaveCount += 1; //Increment WaveCount
			queue[i].MaxMob = MaxMobGrowth; //Update Max Mob limit
			queue[i].WaveCost = CostGrowth;	//Update Value which allows spawning of mobs
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
		free(wtracker[i].tree);
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
