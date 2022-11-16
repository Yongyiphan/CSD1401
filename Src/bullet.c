#pragma once
#include "cprocessing.h"
#include <stdio.h>
#include <math.h>
#include "bullet.h"
#include "utils.h"

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
	for (bulletcounter = 0; bulletcounter < BULLET_CAP; bulletcounter++)
		BulletReset();
}

void BulletCoor(float coordx, float coordy, float angle) //Sets bullet coords
{
	bullet[bulletcounter].x = coordx;
	bullet[bulletcounter].y = coordy;
	bullet[bulletcounter].degree = angle;
}

void BulletType(int type, float coordx, float coordy, float angle, int friendly) // Sets type of bullet and stats
{
	if (type == PBULLET_NORMAL) // Adjust value of 1 into variable after upgrades are made
	{
		BulletReset();
		BulletCoor(coordx, coordy, angle);
		if (friendly == BULLET_MOB) bullet[bulletcounter].friendly = BULLET_MOB; else bullet[bulletcounter].friendly = BULLET_PLAYER;
		bullet[bulletcounter].type = PBULLET_NORMAL;
		bullet[bulletcounter].size = 10 * 1;
		bullet[bulletcounter].speed = 10 * 1;
		bullet[bulletcounter].maxdistance = 200 * 1;
		bullet[bulletcounter].damage = 10 * 1;
		bullet[bulletcounter].exist = TRUE;
	}

	if (type == PBULLET_SPILT) // Triple shot of PBULLET_NORMAL
	{
		for (int i = 0; i < 2; i++) {
			if (i == 0) angle += 15; // Bullet right
			if (i == 1) angle -= 15 * 2; // Bullet left
			BulletType(PBULLET_NORMAL, coordx, coordy, angle, friendly);
			bullet[bulletcounter].type = PBULLET_SPILT;
			bulletcounter++;
		}
	}

	if (type == PBULLET_ROCKET) // Bullet Rocket
	{
		BulletReset();
		BulletCoor(coordx, coordy, angle);
		if (friendly == BULLET_MOB) bullet[bulletcounter].friendly = BULLET_MOB; else bullet[bulletcounter].friendly = BULLET_PLAYER;
		bullet[bulletcounter].type = PBULLET_ROCKET;
		bullet[bulletcounter].size = 15 * 1;
		bullet[bulletcounter].speed = 5 * 1;
		bullet[bulletcounter].maxdistance = 200 * 1;
		bullet[bulletcounter].damage = 3 * 1;
		bullet[bulletcounter].timer = 1;
		bullet[bulletcounter].exist = TRUE;
	}

	if (type == PBULLET_HOMING)
	{
		BulletType(PBULLET_NORMAL, coordx, coordy, angle, friendly);
		bullet[bulletcounter].type = PBULLET_HOMING;
	}
}

void BulletShoot(float coordx, float coordy, float angle, int type, int friendly) // Sets bullet to active
{
	BulletType(type, coordx, coordy, angle, friendly); // Set necessary data to the bullet
	if (bulletcounter < BULLET_CAP) // Adjustments to bullet data should be before this to prevent buffer overrun
		bulletcounter++;
	else bulletcounter = 0;
}

void BulletDirection(float angle, int i) //Determine the bullet directions and travel towards it
{
	float radianangle = CP_Math_Radians(angle);
	double x = cos(radianangle), y = sin(radianangle);
	bullet[i].x += (float)x * bullet[i].speed;
	bullet[i].y += (float)y * bullet[i].speed;
}

void BulletDraw(void) //Draws the location of all active bullets
{

	for (int i = 0; i < BULLET_CAP; i++)
	{
		if (bullet[i].exist == TRUE || bullet[i].type == PBULLET_ROCKET)
		{
			CP_Settings_RectMode(CP_POSITION_CENTER);
			CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
			CP_Graphics_DrawRectAdvanced(bullet[i].x, bullet[i].y, bullet[i].size*2, bullet[i].size, bullet[i].degree, 0);

			if (bullet[i].type == PBULLET_ROCKET && bullet[i].exist == FALSE && bullet[i].timer > 0) // PBULLET_ROCKET explosion draw
			{
				bullet[i].timer += CP_System_GetDt();
				CP_Settings_EllipseMode(CP_POSITION_CENTER);
				CP_Settings_Fill(CP_Color_Create(121, 243, 146, 0));
				CP_Graphics_DrawCircle(bullet[i].x, bullet[i].y, bullet[i].size*10);

				if (bullet[i].timer > 2) { // number - 1 = timer explosion lasts (in seconds)
					bullet[i].timer = 0;
					bullet[i].type = 0;
				}
			}

			if (bullet[i].traveldistance < bullet[i].maxdistance && bullet[i].exist == TRUE)
			{
				BulletDirection(bullet[i].degree, i);
				bullet[i].traveldistance += bullet[i].speed;
			}
			else bullet[i].exist = FALSE;

		}
	
	}
}

int BulletCollision(float targetx, float targety, float width, float height)
{	// Current plan: draws bullet bounderies -> check if mob exist within it -> collide!
	// Current plan: Use circles for all hitbox, try to make it not look off
	float distance = 0;
	for (int i = 0; i < BULLET_CAP; i++) {
		if (bullet[i].type == PBULLET_ROCKET && bullet[i].exist == FALSE && bullet[i].timer == 1) // Explosion Check
		{
			distance = CP_Math_Distance(bullet[i].x, bullet[i].y, targetx, targety);
			if (distance < bullet[i].size * 10)
				return i;
		}

		if (bullet[i].exist == FALSE)
			continue;

		if (bullet[i].type == PBULLET_HOMING) BulletHomingTrack(targetx, targety, width, i);

		CP_Graphics_DrawCircle(targetx, targety, width*0.9); // Draws hitbox zone of mob
		distance = CP_Math_Distance(bullet[i].x, bullet[i].y, targetx, targety);
		if (distance < width*0.9)
			return i;
	}
	return -1; // for no collision with any bullets
}

void BulletHomingTrack(float targetx, float targety, float size, int i)
{
	int homingzone = 3 * size; // Maybe 3* of mob hitbox for homing range?
	if (CP_Math_Distance(bullet[i].x, bullet[i].y, targetx, targety) < homingzone)
	{
		bullet[i].degree = point_point_angle(bullet[i].x, bullet[i].y, targetx, targety);
	}
}
