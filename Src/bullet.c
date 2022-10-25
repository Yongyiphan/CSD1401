
#include "cprocessing.h"
#include <stdio.h>
#include <math.h>

#define BULLET_CAP 50
#define TRUE 1
#define FLASE 0
#define PBULLET_NORMAL 1
#define BULLET_LASER 2

static int bulletcounter = 0;

typedef struct Bullet
{
	float x;
	float y;
	float degree;
	float speed;
	float size;
	float traveldistance;
	float maxdistance;
	float timer;
	int type;
	int friendly;
	int exist;
}Bullet;

Bullet bullet[BULLET_CAP];

void BulletReset() //Reset active bullet
{
	bullet[bulletcounter].exist = FALSE;
	bullet[bulletcounter].traveldistance = 0;
	bullet[bulletcounter].x = 0;
	bullet[bulletcounter].y = 0;
	bullet[bulletcounter].type = 0;
	bullet[bulletcounter].timer = 0;
	bullet[bulletcounter].speed = 0;
	bullet[bulletcounter].size = 0;
	bullet[bulletcounter].maxdistance = 0;
	bullet[bulletcounter].degree = 0;
	bullet[bulletcounter].friendly = 0;
}

void Bulletinit() //Include this in mapinit (Reset for all bullets)
{
	for (; bulletcounter < BULLET_CAP; bulletcounter++)
		BulletReset();
}

void BulletCoor(float playerx, float playery, float playerangle) //Sets bullet coords
{
	bullet[bulletcounter].x = playerx;
	bullet[bulletcounter].y = playery;
	bullet[bulletcounter].degree = playerangle;
}

void BulletType(int type) //Sets type of bullet and stats
{
	if (type == PBULLET_NORMAL) // Adjust value of 1 into variable after upgrades are made
	{
		BulletReset();
		bullet[bulletcounter].size = 10 * 1;
		bullet[bulletcounter].speed = 10 * 1;
		bullet[bulletcounter].maxdistance = 100 * 1;
		bullet[bulletcounter].friendly = TRUE;
	}
}

void BulletShoot(float playerx, float playery, float playerangle, int type) //Sets bullet to active
{
	bullet[bulletcounter].exist = TRUE;
	if (bulletcounter < BULLET_CAP)
		bulletcounter++;
	else bulletcounter = 0;
	BulletType(type);
	BulletCoor(playerx, playery, playerangle);
	
}

void BulletDirection(float playerangle, int i) //Determine the bullet directions and travel towards it
{
	if (playerangle >= 360)
		playerangle -= 360;
	if (playerangle <= 0)
		playerangle += 360;
	float radianangle = CP_Math_Radians(playerangle);
	double x = cos(radianangle), y = sin(radianangle);
	bullet[i].x += x * bullet[i].speed;
	bullet[i].y += y * bullet[i].speed;
}

void BulletDraw(void) //Draws the location of all active bullets
{

	for (int i = 0; i < BULLET_CAP; i++)
	{
		if (bullet[i].exist == TRUE)
		{
			CP_Settings_RectMode(CP_POSITION_CENTER);
			CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
			CP_Graphics_DrawRectAdvanced(bullet[i].x, bullet[i].y, bullet[i].size, bullet[i].size, bullet[i].degree, 0);
			if (bullet[i].traveldistance < bullet[i].maxdistance)
			{
				BulletDirection(bullet[i].degree, i);
				bullet[i].traveldistance += bullet[i].speed;
			}
			else bullet[i].exist = FALSE;
		}

	}
}

