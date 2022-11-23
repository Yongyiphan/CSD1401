#include "cprocessing.h"
#include "credits.h"
#include "utils.h"
#include "mainmenu.h"


#define WINDOWSIZEX 1300
#define WINDOWSIZEY 900

float windowWidth, windowHeight;
float rectWidth, rectHeight;
CP_Vector middle;
CP_Color color, black, white, green, dark_grey;
CP_Image credits, ournames, group_name;
void Credits_Init(void) {
	CP_System_SetWindowSize(WINDOWSIZEX, WINDOWSIZEY);
	windowWidth = CP_System_GetWindowWidth();
	windowHeight = CP_System_GetWindowHeight();
	middle = CP_Vector_Set(windowWidth / 2, windowHeight / 2);
	color = CP_Color_Create(100, 177, 80, 255);
	black = CP_Color_Create(0, 0, 0, 255);
	dark_grey = CP_Color_Create(50, 30, 30, 255);
	white = CP_Color_Create(255, 255, 255, 255);
	green = CP_Color_Create(50, 150, 50, 255);

	rectWidth = 250, rectHeight = 100;

	credits = CP_Image_Load("./Assets/Credits/credits.png");
	ournames = CP_Image_Load("./Assets/Credits/ournames.png");
	group_name = CP_Image_Load("./Assets/Credits/smeg.png");
}

void Credits_Update(void) {
	CP_Graphics_ClearBackground(color);
	CP_Settings_Fill(black);
	
	// Draw out "Credits", group name "SMEG" and team member's names
	CP_Image_Draw(credits, middle.x, windowHeight * 2.0 / 10, CP_Image_GetWidth(credits), CP_Image_GetHeight(credits), 255);
	CP_Image_Draw(group_name, middle.x, windowHeight * 4.0 / 10, CP_Image_GetWidth(group_name), CP_Image_GetHeight(group_name), 255);
	CP_Image_Draw(ournames, middle.x, windowHeight * 5.5 / 10, CP_Image_GetWidth(ournames) * 6.0 / 10, CP_Image_GetHeight(ournames) * 6.0 / 10, 255);
	
	// Draw Exit button
	CP_Settings_StrokeWeight(0.0f);
	CP_Settings_Fill(green);
	CP_Graphics_DrawRectAdvanced(middle.x, windowHeight * 7.5 / 10, rectWidth, rectHeight, 0, 20);
	CP_Settings_Fill(dark_grey);
	CP_Settings_TextSize(60.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText("Exit", middle.x, windowHeight * 7.5 / 10);

	// Exit button
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
		if (IsAreaClicked(middle.x, windowHeight * 7.5 / 10, rectWidth, rectHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		}
	}
}

void Credits_Exit(void) {
	CP_Image_Free(credits);
	CP_Image_Free(ournames);
	CP_Image_Free(group_name);


}