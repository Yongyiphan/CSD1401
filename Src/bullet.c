#pragma once
#include "cprocessing.h"
#include <stdio.h>
#include <math.h>
#include "bullet.h"
#include "utils.h"
#include "player.h"

// Variables declarations
static int bulletcounter = 0;
CP_Image bulletimg = NULL, bulletimg1 = NULL;

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
	for (bulletcounter = 0; bulletcounter < (BULLET_CAP - 1); bulletcounter++)
		BulletReset();
}

void BulletCoor(float coordx, float coordy, float angle) //Sets bullet coords and direction
{
	bullet[bulletcounter].x = coordx;
	bullet[bulletcounter].y = coordy;
	bullet[bulletcounter].degree = angle;
}


void BulletType(int type, float coordx, float coordy, float angle, int friendly) // Sets the type of bullet and relevant stats
{
	if (type == PBULLET_NORMAL) // Settings for default bullet type
	{
		BulletReset();
		BulletCoor(coordx, coordy, angle);
		if (friendly == BULLET_MOB) bullet[bulletcounter].friendly = BULLET_MOB; else bullet[bulletcounter].friendly = BULLET_PLAYER;
		bullet[bulletcounter].type = PBULLET_NORMAL;
		bullet[bulletcounter].size = 10;
		bullet[bulletcounter].speed = 30 * P.STATTOTAL.PROJECTILE_SPD_TOTAL;
		bullet[bulletcounter].maxdistance = 300;
		bullet[bulletcounter].damage = 5 + P.STATTOTAL.DAMAGE_TOTAL;
		bullet[bulletcounter].exist = TRUE;
	}

	if (type == PBULLET_SPILT) // Settings for bullet spilt of PBULLET_NORMAL
	{
		for (int i = 0; i < 2; i++) {
			if (i == 0) angle += 15; // Bullet right
			if (i == 1) angle -= 15 * 2; // Bullet left
			// Self call function to update bullet type to same stats as normal bullet
			BulletType(PBULLET_NORMAL, coordx, coordy, angle, friendly);
			bullet[bulletcounter].type = PBULLET_SPILT;
			bulletcounter++;
		}
	}

	if (type == PBULLET_ROCKET) // Settings for bullet type rocket (looks like a mine)
	{
		BulletReset();
		BulletCoor(coordx, coordy, angle);
		if (friendly == BULLET_MOB) bullet[bulletcounter].friendly = BULLET_MOB; else bullet[bulletcounter].friendly = BULLET_PLAYER;
		bullet[bulletcounter].type = PBULLET_ROCKET;
		bullet[bulletcounter].size = 15;
		bullet[bulletcounter].speed = 5 * P.STATTOTAL.PROJECTILE_SPD_TOTAL;
		bullet[bulletcounter].maxdistance = 300;
		bullet[bulletcounter].damage = 10 + P.STATTOTAL.DAMAGE_TOTAL;
		bullet[bulletcounter].timer = 1;
		bullet[bulletcounter].exist = TRUE;
	}

	if (type == PBULLET_HOMING) // Settings for bullet type homing, uses same stats as normal bullet
	{
		BulletType(PBULLET_NORMAL, coordx, coordy, angle, friendly);
		bullet[bulletcounter].type = PBULLET_HOMING;
	}
}

void BulletShoot(float coordx, float coordy, float angle, int type, int friendly) // Sets the bullet to active
{
	BulletType(type, coordx, coordy, angle, friendly); // Set necessary data to the bullet
	if (bulletcounter < (BULLET_CAP - 1)) // Adjustments to bullet data should be before this to prevent buffer overrun
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
	// Checks all bullets
	for (int i = 0; i < BULLET_CAP; i++)
	{
		if (bullet[i].exist == TRUE || bullet[i].type == PBULLET_ROCKET)
		{
			// Bullet image settings
			CP_Settings_ImageMode(CP_POSITION_CENTER);
			CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
			float IWidth = CP_Image_GetWidth(bulletimg) / 3, IHeight = CP_Image_GetHeight(bulletimg);
			float Imgsize = 30;

			// Draw image for normal/spilt bullets
			if (bullet[i].type == PBULLET_NORMAL || bullet[i].type == PBULLET_SPILT)
			CP_Image_DrawSubImage(bulletimg, bullet[i].x, bullet[i].y, bullet[i].size * 2, bullet[i].size * 2,
				IWidth * 0, 0, IWidth * 0 + IWidth, IHeight, 255);

			// Draw image for explosive bullet
			if (bullet[i].type == PBULLET_ROCKET && bullet[i].exist == TRUE)
				CP_Image_DrawSubImage(bulletimg, bullet[i].x, bullet[i].y, Imgsize, Imgsize,
					IWidth * 1, 0, IWidth * 1 + IWidth, IHeight, 255);

			// Draw image for homing bullet
			if (bullet[i].type == PBULLET_HOMING)
				CP_Image_DrawSubImage(bulletimg, bullet[i].x, bullet[i].y, Imgsize, Imgsize,
					IWidth * 2, 0, IWidth * 2 + IWidth, IHeight, 255);

			// Draw the image for the "explosion" of the explosive bullet
			if (bullet[i].type == PBULLET_ROCKET && bullet[i].exist == FALSE && bullet[i].timer > 0)
			{
				// Timer for explosion image duration
				bullet[i].timer += CP_System_GetDt();
				CP_Image_DrawSubImage(bulletimg1, bullet[i].x, bullet[i].y, CP_Image_GetWidth(bulletimg1), CP_Image_GetHeight(bulletimg1),
					0, 0, CP_Image_GetWidth(bulletimg1) , IHeight, 255);

				if (bullet[i].timer > 2) { // number - 1 => timer explosion effects lasts (in seconds)
					bullet[i].timer = 0;
					bullet[i].type = 0;
				}
			}

			// Check when bullet reaches the set maximum distance and set it to no longer exist
			if (bullet[i].traveldistance < bullet[i].maxdistance && bullet[i].exist == TRUE)
			{
				BulletDirection(bullet[i].degree, i);
				bullet[i].traveldistance += bullet[i].speed;
			}
			else bullet[i].exist = FALSE;

		}
	
	}
}

// Check when a bullet collides with a target
int BulletCollision(float targetx, float targety, float width, float height)
{	
	float distance = 0;
	for (int i = 0; i < BULLET_CAP; i++) {

		// Check when a target is within the range of the explosive bullet
		if (bullet[i].type == PBULLET_ROCKET && bullet[i].exist == FALSE && bullet[i].timer == 1)
		{
			distance = CP_Math_Distance(bullet[i].x, bullet[i].y, targetx, targety);
			if (distance < bullet[i].size * 10)
				return i;
		}

		// Skip to next bullet when current bullet does not exist
		if (bullet[i].exist == FALSE)
			continue;

		// Calls function for homing bullet to track mob that is within tracking range
		if (bullet[i].type == PBULLET_HOMING) BulletHomingTrack(targetx, targety, width, i);

		// Checks collision within circle
		distance = CP_Math_Distance(bullet[i].x, bullet[i].y, targetx, targety);
		if (distance < width*0.9)
			return i;
	}
	return -1; // for no collision with any bullets
}

// Re-set the bullet direction towards the mob
void BulletHomingTrack(float targetx, float targety, float size, int i)
{
	int homingzone = 3 * size; // Current setting is 3 times of mob hitbox for homing range
	if (CP_Math_Distance(bullet[i].x, bullet[i].y, targetx, targety) < homingzone)
	{
		bullet[i].degree = point_point_angle(bullet[i].x, bullet[i].y, targetx, targety);
	}
}

// Update the timer array 
void UpdateCDTimer(float* bulletcd, int size)
{
	float timeinc = CP_System_GetDt();
	for (int i = 0; i < size; i++, bulletcd += 1)
		*bulletcd += timeinc;
}

// Load images assets
void BulletImgLoad(void)
{
	bulletimg = CP_Image_Load("./Assets/Bullets/bulletspritesheet.png");
	bulletimg1 = CP_Image_Load("./Assets/Bullets/bulletexplode.png");
}

// Free images assets
void BulletImgFree(void)
{
	CP_Image_Free(&bulletimg);
	CP_Image_Free(&bulletimg1);
}



