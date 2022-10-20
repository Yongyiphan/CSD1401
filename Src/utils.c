#include "cprocessing.h"
#include <stdio.h>
#include "game.h"

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
		printf("%d\t%d\t%d\t%d\n", leftLimit, rightLimit, topLimit, bottomLimit);
		printf("%f\t%f", click_x, click_y);
		return 1;
	}
	return 0;
}

void option_screen(int* isPaused) {
	float screen_width = CP_System_GetDisplayWidth();
	float screen_height = CP_System_GetDisplayHeight();
	CP_Vector middle = CP_Vector_Set(screen_width / 2, screen_height / 2);

	int width = 300;
	int height = 60;
	int padding = 30;

	float textSize = 30.0;

	CP_Settings_RectMode(CP_POSITION_CENTER);

	CP_Settings_NoStroke();
	// Options background
	CP_Settings_Fill(CP_Color_Create(80, 80, 100, 120));
	CP_Graphics_DrawRect(middle.x, middle.y, screen_width * 4.0 / 10, screen_height * 8.0 / 10);

	// Draw out option boxes
	CP_Settings_Fill(CP_Color_Create(255, 100, 100, 255));
	CP_Graphics_DrawRect(middle.x, middle.y, width, height);
	CP_Graphics_DrawRect(middle.x, middle.y + height + padding, width, height);

	// Align text to the center vertically and horizontally
	CP_TEXT_ALIGN_HORIZONTAL centerHor = CP_TEXT_ALIGN_H_CENTER;
	CP_TEXT_ALIGN_VERTICAL centerVert = CP_TEXT_ALIGN_V_MIDDLE;
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));		// text is black

	// Draw text of respective boxes at respective coordinates.
	CP_Settings_TextSize(40.0f);
	CP_Settings_TextAlignment(centerHor, centerVert);
	CP_Font_DrawText("Resume Game", middle.x, middle.y);
	CP_Font_DrawText("Exit to main menu", middle.x, middle.y + height + padding);


	// Remove rectangle align-center and add stroke back in
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));

	
	if (CP_Input_MouseClicked()) {
		if (IsAreaClicked(middle.x, middle.y, width, height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			*isPaused = 0;
		}
		if (IsAreaClicked(middle.x, middle.y + height + padding, width, height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Engine_SetNextGameState(game_Init, game_Update, game_Exit);
		}
	}
	
}