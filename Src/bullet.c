#pragma once
#include "cprocessing.h"
#include <stdio.h>
#include <math.h>
#include "bullet.h"

static int bulletcounter = 0;

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
	bullet[bulletcounter].damage = 0;
}

void Bulletinit() //Include this in mapinit (Reset for all bullets)
{
	for (; bulletcounter < BULLET_CAP; bulletcounter++)
		BulletReset();
}

void BulletCoor(float coordx, float coordy, float angle) //Sets bullet coords
{
	bullet[bulletcounter].x = coordx;
	bullet[bulletcounter].y = coordy;
	bullet[bulletcounter].degree = angle;
}

void BulletType(int type) //Sets type of bullet and stats
{
	if (type == PBULLET_NORMAL) // Adjust value of 1 into variable after upgrades are made
	{
		bullet[bulletcounter].size = 10 * 1;
		bullet[bulletcounter].speed = 10 * 1;
		bullet[bulletcounter].maxdistance = 100 * 1;
		bullet[bulletcounter].damage = 10 * 1;
	}
}

void BulletShoot(float coordx, float coordy, float angle, int type, int friendly) //Sets bullet to active
{
	BulletReset();
	BulletType(type);
	BulletCoor(coordx, coordy, angle);
	bullet[bulletcounter].exist = TRUE;
	if (friendly == BULLET_MOB) bullet[bulletcounter].friendly = BULLET_MOB; 
	else bullet[bulletcounter].friendly = BULLET_PLAYER;
	if (bulletcounter < BULLET_CAP) //Adjustments to bullet data should be before this to prevent buffer overrun
		bulletcounter++;
	else bulletcounter = 0;
}

void BulletDirection(float angle, int i) //Determine the bullet directions and travel towards it
{
	float radianangle = CP_Math_Radians(angle);
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

int BulletCollision(float targetx, float targety, float size)
{
	float distance = 0;
	for (int i = 0; i < BULLET_CAP; i++) {
		if (bullet[i].exist == FALSE)
			continue;
		distance = CP_Math_Distance(bullet[i].x, bullet[i].y, targetx, targety);
		if (distance < size)
			return i;
	}
	return -1; // for no collision
}

