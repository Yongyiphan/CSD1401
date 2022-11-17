#pragma once
#ifndef BULLET_H
#define BULLET_H


#define BULLET_CAP 50
#define BULLET_MOB 0
#define BULLET_PLAYER 1
#define TRUE 1
#define FALSE 0
#define PBULLET_NORMAL 1
#define PBULLET_SPILT 2
#define PBULLET_ROCKET 3
#define PBULLET_HOMING 4 

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

// Sets the data for bullet type presets (type, x, y, angle, friendly)
void BulletType(int, float, float, float, int);

// Calculate the direction of the bullet (angle, bulletid)
void BulletDirection(float, int);
*/


// Call to shoot bullet (playerx, playery, angle, type, bulletowner)
void BulletShoot(float, float, float, int, int);

// Draws the location of all bullets
void BulletDraw(void);

// Check existing bullets collision against a target (x, y, w, h)
int BulletCollision(float, float, float, float);

// Updates homing bullet angle to lock onto mob
void BulletHomingTrack(float targetx, float targety, float size, int i);

#endif
