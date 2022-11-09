#pragma once
#include "cprocessing.h"
#include <stdio.h>
#include <math.h>
#include "bullet.h"
#include "utils.h"

//static int bulletcounter = 0;
#define BULLET_WIDTH 10
#define BULLET_HEIGHT 20


Bullet BulletList[BulletArrSize];
//void Bullet_Init(Bullet bullets[], int size, Player P) //Include this in mapinit (Reset for all bullets)
void Bullet_Init(int size, Player P) {
	for (int i = 0; i < size; i++) {
		Bullet bullet = BulletList[i];
		bullet.state = INACTIVE;
		bullet.velocity = CP_Vector_Set(0, 0);
		bullet.coord = CP_Vector_Set(P.x, P.y);
		bullet.id = i;
		bullet.width = BULLET_WIDTH;
		bullet.height = BULLET_HEIGHT;
		bullet.degree = 0;
	}
}

void Bullet_Reset(Bullet *bullet, Player P) //Reset active bullet
{
	bullet->state = INACTIVE;
	bullet->velocity = CP_Vector_Set(0, 0);
	bullet->coord = CP_Vector_Set(P.x, P.y);
	bullet->degree = 0;
}

//void BulletCoor(float coordx, float coordy, float angle) //Sets bullet coords
//{
//	bullet[bulletcounter].x = coordx;
//	bullet[bulletcounter].y = coordy;
//	bullet[bulletcounter].degree = angle;
//}

//void BulletType(int type) //Sets type of bullet and stats
//{
//	if (type == PBULLET_NORMAL) // Adjust value of 1 into variable after upgrades are made
//	{
//		bullet[bulletcounter].size = 10 * 1;
//		bullet[bulletcounter].speed = 30 * 1;
//		bullet[bulletcounter].maxdistance = 1000 * 1;
//		bullet[bulletcounter].damage = 10 * 1;
//	}
//}

void Bullet_Update(int bullet_count, Player player) {
	
	for (int i = 0; i < bullet_count; i++) {
		Bullet *bullet = &BulletList[i];
		if (bullet->state) {
			bullet->coord = CP_Vector_Add(bullet->coord, CP_Vector_Scale(bullet->velocity, CP_System_GetDt()));
			printf("");
		}
		
	}
}

//Sets bullet to active
//Bullet* Bullet_Spawn(Bullet bullets[], int count, Player P, CP_Vector shoot_direction) 
void Bullet_Spawn(int count, Player P, CP_Vector shoot_direction)
{

	CP_Vector right = CP_Vector_Set(1, 0);
	CP_Vector up = CP_Vector_Set(0, 1);

	// Normalize sets the vector to be units 0~1.
	shoot_direction = CP_Vector_Normalize(CP_Vector_Set(shoot_direction.x - P.x, shoot_direction.y - P.y));

	float rotate = CP_Vector_Angle(shoot_direction, right);
	if (CP_Vector_DotProduct(shoot_direction, up) < 0)
		rotate = -rotate;

	int ifAllSpawned = 0;
	for (int i = 0; i < count; i++)
	{
		Bullet bullet = BulletList[i];
		
		if (!bullet.state)
		{
			bullet.coord = CP_Vector_Set(P.x, P.y);
			bullet.degree = rotate;
			//bullet.velocity = CP_Vector_Set(shoot_direction.x * P.STATTOTAL.PROJECTILE_SPEED_TOTAL , shoot_direction.y * P.STATTOTAL.PROJECTILE_SPEED_TOTAL);
			bullet.velocity = CP_Vector_Scale(shoot_direction, P.STATTOTAL.PROJECTILE_SPEED_TOTAL * 4);

			bullet.state = ACTIVE;
			//bullet.type = LINEAR_PROJECTILE;

			BulletList[i] = bullet;
			break;
		}
	}

}

// These functions are for ranged units that attack players
// Due to time constraints, they will not be implemented.
/*int Bullet_Collide(Player p, Bullet bullet) {
	if (CP_Math_Distance(p.x, p.y, bullet.coord.x, bullet.coord.y) < p.HITBOX / 2 + bullet.maxdistance / 2) {
		p.CURRENT_HP -= 5;
		return 1;
	}
	return 0;
}*/

// Bullet collides with target
void Bullet_Collision(Mob *mob, int count, Player P)
{
	for (int i = 0; i < count; i++) {
		float bulletX = BulletList[i].coord.x;
		float bulletY = BulletList[i].coord.y;
		float distance = CP_Math_Distance( bulletX, bulletY, mob->x, mob->y);
		//printf("Bullet:\nX: %f\tY: %f\t%f\n", bulletX, bulletY, distance);
		if (distance <= BulletList[i].size + mob->BaseStats.size) {
			mob->CStats.HP -= P.STATTOTAL.DAMAGE_TOTAL;
			printf("Mob has collided with bullet!\n");
			Bullet_Reset(&BulletList[i], P);
			
		}
		float half_width = CP_System_GetWindowWidth() / 2;
		float half_height = CP_System_GetWindowHeight() / 2;
		// works
		if (bulletX > P.x + half_width || bulletX < P.x - half_width || bulletY > P.y + half_height || bulletY < P.y - half_height) {
			Bullet_Reset(&BulletList[i], P);
		}
		
	}
	
}

//void BulletDirection(float angle, int i) //Determine the bullet directions and travel towards it
//{
//	float radianangle = CP_Math_Radians(angle);
//	double x = cos(radianangle), y = sin(radianangle);
//	bullet[i].x += x * bullet[i].speed;
//	bullet[i].y += y * bullet[i].speed;
//}

void Bullet_Draw(int count) //Draws the location of all active bullets
{
	for (int i = 0; i < count; i++)
	{
		//Bullet bullet = arr_bullet[i];
		Bullet bullet = BulletList[i];
		/*if (bullet.state) {
			CP_Image_DrawAdvanced(bullet_sprite, bullet.coord.x, bullet.coord.y, BULLET_WIDTH, BULLET_HEIGHT, 255, bullet.degree);
		}*/
		if (bullet.state) {
			CP_Graphics_DrawRectAdvanced(bullet.coord.x, bullet.coord.y, BULLET_WIDTH, BULLET_HEIGHT, bullet.degree, 5);
		}
	}
}



