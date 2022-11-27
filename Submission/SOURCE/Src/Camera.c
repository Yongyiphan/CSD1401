#pragma once
#include "CProcessing.h"
#include <stdio.h>
#include "player.h"

/* --------------------------------------
* File Level Documentation
* @author	Geoffrey Cho Jian Ming
* @email	g.cho@digipen.edu
* @file		Camera.c
* @brief	This file contains all functions required for camera movement
			- Displaces any object placed below the camera function
			- Fixes the player to the middle of the screen
* Copyright 2022 Digipen, All Rights Reserved.
*//*-------------------------------------*/

static const float SCALE_QUANTUM = 1.5f;
static const float ROTATION_QUANTUM = 5.0f;
static const float PAN_QUANTUM = 200.0f;
static const float FONT_SIZE = 50.0f;

static float currentScale;
static float currentRotation;
static CP_Vector currentPosition, centerOffset;

static CP_Matrix scaleMatrix, rotationMatrix, translationMatrix;

void CameraDemo_Init(void)
{
	scaleMatrix = rotationMatrix = CP_Matrix_Identity();
	currentScale = 1.0f;
	currentRotation = 0.0f;

	float centerX = CP_System_GetWindowWidth() / 2.0f;
	float centerY = CP_System_GetWindowHeight() / 2.0f;
	centerOffset = CP_Vector_Set(centerX, centerY);
	currentPosition = CP_Vector_Zero();
	translationMatrix = CP_Matrix_Translate(currentPosition);

	CP_Settings_TextSize(FONT_SIZE);
}

void CameraDemo_Displace_Update(void) {
	float dt = CP_System_GetDt();
}

void CameraDemo_Update(Player *player, CP_Matrix *trans)
{
	float dt = CP_System_GetDt();


	// THIS IS ALL CAMERA MOVEMENT, ARROW KEYS UP DOWN LEFT RIGHT FOR CAMERA CONTROL
	
	if (CP_Input_KeyDown(KEY_W))
	{
		currentPosition.y += dt * player->STATTOTAL.SPEED_TOTAL;
		translationMatrix = CP_Matrix_Translate(currentPosition);
	}
	if (CP_Input_KeyDown(KEY_S))
	{
		currentPosition.y += dt * -player->STATTOTAL.SPEED_TOTAL;
		translationMatrix = CP_Matrix_Translate(currentPosition);
	}
	if (CP_Input_KeyDown(KEY_D))
	{
		currentPosition.x += dt * -player->STATTOTAL.SPEED_TOTAL;
		translationMatrix = CP_Matrix_Translate(currentPosition);
	}
	if (CP_Input_KeyDown(KEY_A))
	{
		currentPosition.x += dt * player->STATTOTAL.SPEED_TOTAL;
		translationMatrix = CP_Matrix_Translate(currentPosition);
	}

	CP_Vector offsetOrigin = CP_Vector_Scale(currentPosition, -1.0f);
	CP_Vector offsetVector = CP_Vector_Add(offsetOrigin, centerOffset);
	CP_Matrix offsetMatrix = CP_Matrix_Translate(CP_Vector_Scale(offsetVector, -1.0f));

	//translate all objects by the world space position of the current screen center
	CP_Matrix offsetUndoMatrix = CP_Matrix_Translate(offsetVector);
	//now all objects are within the screen center's local coord system
	//so we apply the scale
	CP_Matrix transform = CP_Matrix_Multiply(scaleMatrix, offsetMatrix);
	//then the rotation
	transform = CP_Matrix_Multiply(rotationMatrix, transform);
	//then we return all objects to the world space
	transform = CP_Matrix_Multiply(offsetUndoMatrix, transform);
	//translate objects in world space based on the current camera panning
	transform = CP_Matrix_Multiply(translationMatrix, transform);

	*trans = transform;
	CP_Settings_ApplyMatrix(transform);

	//Draw environment props that do not move
	CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText("W A S D to move", 450, 300);
	CP_Font_DrawText("Hold Left Mouse Click to Shoot", 450, 200);

	//this draws the center circle in which the scale and rotation transformations
	//take reference from

	/* THIS IS THE PLAYER!!! */
	CP_Settings_Fill(CP_Color_Create(0, 0, 255, 255));
	player->x = offsetVector.x;
	player->y = offsetVector.y;
	player->coor = offsetVector;
	CP_Graphics_DrawCircle(player->x, player->y, 50.0f);
}