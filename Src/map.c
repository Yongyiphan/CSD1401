#include "cprocessing.h"
#include <stdio.h>
#include <math.h>
#include "bullet.h"

#define MAP_SIZE 700
#define PLAYER_HITBOX 50
#define PLAYER_SPEED 10
#define PLAYER_ROT_SPEED 5

typedef struct Player {
	float x;
	float y;
	float angle;
}Player;

Player p;
CP_Vector p_vector;
CP_Color grey, black, red, blue, white;

// Return a vector using a given radian angle
CP_Vector AngleToVector(float radian_angle)
{
	// TODO
	CP_Vector ret = CP_Vector_Set(cos(radian_angle), sin(radian_angle));
	return ret;
}

void player_movement(CP_Vector p_vector) {
	if (CP_Input_KeyDown(KEY_W)) {
		p.x += p_vector.x * PLAYER_SPEED;
		p.y += p_vector.y * PLAYER_SPEED;
		printf("x: %.2f, y: %.2f, degrees: %.2f\n", p.x, p.y, p.angle);
	}
	if (CP_Input_KeyDown(KEY_A)) {
		p.angle -= PLAYER_ROT_SPEED;
		printf("x: %.2f, y: %.2f, degrees: %.2f\n", p.x, p.y, p.angle);
	}
	if (CP_Input_KeyDown(KEY_S)) {
		p.x -= p_vector.x * PLAYER_SPEED;
		p.y -= p_vector.y * PLAYER_SPEED;
		printf("x: %.2f, y: %.2f, degrees: %.2f\n", p.x, p.y, p.angle);
	}
	if (CP_Input_KeyDown(KEY_D)) {
		p.angle += PLAYER_ROT_SPEED;
		printf("x: %.2f, y: %.2f, degrees: %.2f\n", p.x, p.y, p.angle);
	}
}

void map_Init(void) {
	CP_System_SetWindowSize(MAP_SIZE, MAP_SIZE);
	p.x = 200;
	p.y = 400;
	p.angle = 30;
	

	grey = CP_Color_Create(111, 111, 111, 255);
	white = CP_Color_Create(255, 255, 255, 255);

	Bulletinit();
}

void map_Update(void) {
	p_vector = AngleToVector(CP_Math_Radians(p.angle));
	CP_Graphics_ClearBackground(grey);

	CP_Settings_Fill(white);
	CP_Graphics_DrawEllipseAdvanced(p.x, p.y, PLAYER_HITBOX, PLAYER_HITBOX, p.angle);
	
	// Directional Inputs for player
	player_movement(p_vector);

	//Test shoot
	if (CP_Input_KeyDown(KEY_SPACE))
	{
		BulletShoot(p.x, p.y, p.angle, 1);
	}
	BulletDraw();

}

void map_Exit(void) {

}