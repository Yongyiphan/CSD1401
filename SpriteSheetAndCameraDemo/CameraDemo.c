#include "CProcessing.h"
#include <stdio.h>

static const float SCALE_QUANTUM = 1.5f;
static const float ROTATION_QUANTUM = 5.0f;
static const float PAN_QUANTUM = 100.0f;
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

void CameraDemo_Update(void)
{
	float dt = CP_System_GetDt();

	if (CP_Input_KeyDown(KEY_R))
	{
		currentRotation += dt * ROTATION_QUANTUM;
		rotationMatrix = CP_Matrix_Rotate(currentRotation);
	}
	else if (CP_Input_KeyDown(KEY_T))
	{
		currentRotation += dt * -ROTATION_QUANTUM;
		rotationMatrix = CP_Matrix_Rotate(currentRotation);
	}

	if (CP_Input_KeyDown(KEY_I))
	{
		currentScale += dt * SCALE_QUANTUM;
		scaleMatrix = CP_Matrix_Scale(CP_Vector_Set(currentScale, currentScale));
	}
	else if (CP_Input_KeyDown(KEY_O))
	{
		currentScale += dt * - SCALE_QUANTUM;
		scaleMatrix = CP_Matrix_Scale(CP_Vector_Set(currentScale, currentScale));
	}

	if (CP_Input_KeyDown(KEY_UP))
	{
		currentPosition.y += dt * PAN_QUANTUM;
		translationMatrix = CP_Matrix_Translate(currentPosition);
		printf("Current Pos: %f %f\n", currentPosition.x, currentPosition.y);
	}
	else if (CP_Input_KeyDown(KEY_DOWN))
	{
		currentPosition.y += dt * -PAN_QUANTUM;
		translationMatrix = CP_Matrix_Translate(currentPosition);
		printf("Current Pos: %f %f\n", currentPosition.x, currentPosition.y);
	}
	else if (CP_Input_KeyDown(KEY_RIGHT))
	{
		currentPosition.x += dt * -PAN_QUANTUM;
		translationMatrix = CP_Matrix_Translate(currentPosition);
		printf("Current Pos: %f %f\n", currentPosition.x, currentPosition.y);
	}
	else if (CP_Input_KeyDown(KEY_LEFT))
	{
		currentPosition.x += dt * PAN_QUANTUM;
		translationMatrix = CP_Matrix_Translate(currentPosition);
		printf("Current Pos: %f %f\n", currentPosition.x, currentPosition.y);
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

	CP_Settings_ApplyMatrix(transform);

	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 0));
	//Draw environment props that do not move
	CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
	CP_Font_DrawText("Hello", 255, 255);
	CP_Font_DrawText("What you looking at?", 450, 200);
	CP_Font_DrawText("No jokes about chuck norris", 750, 20);

	//this draws the center circle in which the scale and rotation transformations
	//take reference from
	CP_Settings_Fill(CP_Color_Create(0, 0, 255, 255));
	CP_Graphics_DrawCircle(offsetVector.x, offsetVector.y, 50.0f);
}