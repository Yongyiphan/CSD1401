#include "cprocessing.h"
#include "credits.h"
#include "utils.h"
#include "mainmenu.h"


/* --------------------------------------
* File Level Documentation
* @author		Geoffrey Cho Jian Ming
* @email		g.cho@digipen.edu
* @contributor	Edgar Yong
* @file			Credits.c
* @brief		This file draws the credit page

* Copyright 2022 Digipen, All Rights Reserved.
*//*-------------------------------------*/


#define WINDOWSIZEX 1300
#define WINDOWSIZEY 900

float windowWidth, windowHeight;
float rectWidth, rectHeight;
CP_Vector middle;
CP_Color color, black, green, dark_grey;
CP_Image rollingcredit;
int rollingspeed = 5;
int offset;
void Credits_Init(void) {
	CP_System_SetWindowSize(WINDOWSIZEX, WINDOWSIZEY);
	windowWidth = (float) CP_System_GetWindowWidth();
	windowHeight = (float) CP_System_GetWindowHeight();
	middle = CP_Vector_Set(windowWidth / 2, windowHeight / 2);
	color = CP_Color_Create(100, 177, 80, 255);
	black = CP_Color_Create(0, 0, 0, 255);
	dark_grey = CP_Color_Create(50, 30, 30, 255);
	green = CP_Color_Create(50, 150, 50, 255);

	rectWidth = 150, rectHeight = 50;

	rollingcredit = CP_Image_Load("./Assets/Credits/RollingCredit-1 (1).png");
	offset = 0;
}
void Credits_Update(void) {

	CP_Settings_Save();
	CP_Graphics_ClearBackground(color);
	CP_Settings_Fill(black);
	offset += rollingspeed;
	CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_REPEAT);
	CP_Settings_ImageMode(CP_POSITION_CENTER);
	CP_Image_DrawSubImage(rollingcredit, middle.x, middle.y, 
		windowWidth, windowHeight, 
		0.0f, 0.0f + offset, (float) CP_Image_GetWidth(rollingcredit), (float) windowHeight + offset, 255);
	
	//Draw Exit Button
	CP_Settings_StrokeWeight(0.0f);
	CP_Settings_Fill(green);
	CP_Vector exitbtn = CP_Vector_Set(windowWidth * 11 / 12, windowHeight * 9 / 10);
	CP_Graphics_DrawRectAdvanced(exitbtn.x, exitbtn.y, rectWidth, rectHeight, 0, 20);
	CP_Settings_Fill(dark_grey);
	CP_Settings_TextSize(50.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText("Exit", exitbtn.x, exitbtn.y);
	CP_Settings_Restore();
	// Exit button
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
		if (IsAreaClicked(exitbtn.x,exitbtn.y, rectWidth, rectHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		}
	}
	if (CP_Input_KeyTriggered(KEY_ESCAPE)) 
	{
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update,Main_Menu_Exit);
	}
}

void Credits_Exit(void) {
	CP_Image_Free(&rollingcredit);
}
