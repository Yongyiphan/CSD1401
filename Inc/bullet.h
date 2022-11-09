#pragma once
#include "cprocessing.h"
#include "Mob.h"
#include "player.h"

#ifndef BULLET_H
#define BULLET_H


#define ACTIVE 1
#define INACTIVE 0

typedef struct Bullet
{
	CP_Vector velocity;
	CP_Vector coord;
	float degree;
	float speed;
	float size;
	float damage;
	int state;
	int id;

	float width;
	float height;
}Bullet;

#define BulletArrSize 500
extern Bullet BulletList[BulletArrSize];
// Reset the bullet and change its state to inactive
void Bullet_Reset(Player P);

// Initialize all bullets at the start of the game
void Bullet_Init(int size, Player P);

/*
// Sets the bullet Coords and angle (x,y,degree)
void BulletCoor(float, float, float);

// Sets the data for bullet type presets
void BulletType(int);

// Calculate the direction of the bullet
void BulletDirection(float, int);
*/


// Shoots bullet and returns the bullet that had just been turned active
void Bullet_Update(int count, Player P);

// Spawn bullet
void Bullet_Spawn(int count, Player P, CP_Vector shoot_direction);

// Draws all bullets
void Bullet_Draw(int count);

// Check collision against mob
// If collided, the mob is damaged according to player damage
void Bullet_Collision(Mob* mob, int count, Player P);


#endif
