#pragma once
#include "cprocessing.h"
#include <stdio.h>
#include <math.h>
#include "player.h"
#include "game.h"
#include "mainmenu.h"
#include "audio_manager.h"

#define _countof(array) (sizeof(array) / sizeof(array[0]))

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y)
{
	// TODO
	// within x dimensions of the given area
	int leftLimit = area_center_x - area_width / 2;
	int rightLimit = area_center_x + area_width / 2;
	int topLimit = area_center_y - area_height / 2;
	int bottomLimit = area_center_y + area_height / 2;

	// if x-coordinate of click is within left and right of the area, and 
	// y-coordinate is within top and bottom of the area, then return 1. If not, return 0.
	if (leftLimit < click_x && click_x < rightLimit && topLimit < click_y && click_y < bottomLimit) {
		//Audio_ButtonClick();
		return 1;
	}
	return 0;
}

void option_screen(int* isPaused) {
	float screen_width = CP_System_GetWindowWidth();
	float screen_height = CP_System_GetWindowHeight();
	CP_Vector middle = CP_Vector_Set(screen_width / 2, screen_height / 2);

	int width = 300;
	int height = 60;
	int padding = 30;
	float cornerRadius = 20;

	float textSize = 30.0;

	CP_Settings_RectMode(CP_POSITION_CENTER);

	CP_Settings_NoStroke();
	// Options background
	CP_Settings_Fill(CP_Color_Create(80, 80, 100, 120));

	CP_Graphics_DrawRectAdvanced(middle.x, middle.y, screen_width * 4.0 / 10, screen_height * 8.0 / 10, 0, cornerRadius);

	// Draw out option boxes
	CP_Settings_Fill(CP_Color_Create(255, 100, 100, 255));
	CP_Graphics_DrawRect(middle.x, middle.y, width, height);
	CP_Graphics_DrawRect(middle.x, middle.y + height + padding, width, height);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));		// text is black

	// Draw text of respective boxes at respective coordinates.
	CP_Settings_TextSize(40.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	//CP_Font_DrawTextBox("Upgrades", middle.x, middle.y - height - padding, width);
	CP_Font_DrawText("Resume Game", middle.x, middle.y);
	CP_Font_DrawText("Exit to main menu", middle.x, middle.y + height + padding);


	// Remove rectangle align-center and add stroke back in
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));


	if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
		if (IsAreaClicked(middle.x, middle.y, width, height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			*isPaused = 0;
		}

		if (IsAreaClicked(middle.x, middle.y + height + padding, width, height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		}
	}
}


float timer(int isPaused, int init) {
	
	float currentElapsedTime = CP_System_GetDt();
	static float totalElapsedTime = 0;
	
	if (init) {
		totalElapsedTime = 0;
	}
	if (!isPaused) {
		totalElapsedTime += currentElapsedTime;
		//CP_Font_DrawTextBox("Upgrades", middle.x, middle.y - height - padding, width);
	}
	return totalElapsedTime;
}

// Prints out a countdown timer in the middle of the screen
// If timer has reached 0, return TRUE. Else, return FALSE.
int Timer_CountDown(void) {
	CP_Vector middle = CP_Vector_Set(CP_System_GetWindowWidth() / 2.0, CP_System_GetWindowHeight() / 2.0);
	float currentElapsedTime = CP_System_GetDt();
	static float countDownTime = 3;

	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
	countDownTime -= currentElapsedTime;
	char buffer[16] = { 0 };
	sprintf_s(buffer, _countof(buffer), "%.f", countDownTime);
	CP_Font_DrawText(buffer, middle.x, middle.y);

	if (countDownTime <= 0) {
		countDownTime = 3;
		return TRUE;
	}
	return FALSE;
}

void Draw_Time(float totalElapsedTime) {
	float x = CP_System_GetWindowWidth() / 2;
	float y = CP_System_GetWindowHeight() * 0.8 / 10;

	// Draw text of respective boxes at respective coordinates.
	CP_Settings_TextSize(40.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));

	char buffer[16] = { 0 };
	sprintf_s(buffer, _countof(buffer), "%.2f", totalElapsedTime);
	CP_Font_DrawText("Time: ", x - 50, y);
	CP_Font_DrawText(buffer, x + 50, y);
}

float point_point_angle(float x1, float y1, float x2, float y2)
{
	float tempx=0, tempy=0, temp=0;
	tempx = x2 - x1;
	tempy = y2 - y1;
	temp = atan2(tempy, tempx);
	temp = CP_Math_Degrees(temp);
	return temp;
}
