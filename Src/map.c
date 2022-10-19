#include "cprocessing.h"
#include <stdio.h>
#include <math.h>
#include "Camera.h"
#include "player.h"

#define MAP_SIZE 700
#define PLAYER_HP 100.0f
#define PLAYER_SPEED 200.0f
#define PLAYER_DAMAGE 1.0f
#define ATK_SPEED 2.0f
#define DEFENSE 10


Player P;
CP_Vector start_vector;
CP_Color grey, black, red, blue, white;



void map_Init(void) {
	CP_System_SetWindowSize(MAP_SIZE, MAP_SIZE);

	grey = CP_Color_Create(111, 111, 111, 255);
	white = CP_Color_Create(255, 255, 255, 255);

	start_vector = CP_Vector_Zero();
	// Initialize the coordinates and stats of the player
	P = (Player){ start_vector.x, start_vector.y, PLAYER_HP, PLAYER_SPEED, PLAYER_DAMAGE, ATK_SPEED, DEFENSE };
	CameraDemo_Init();
}

void map_Update(void) {
	
	CameraDemo_Update(P);
	if (CP_Input_KeyDown(KEY_H)) {
		P.SPEED *= 1.1;
	}
}

void map_Exit(void) {

}