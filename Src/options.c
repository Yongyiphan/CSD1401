#pragma once
#include "cprocessing.h"
#include "utils.h"
#include "mainmenu.h"
#include "options.h"

#define WINDOWSIZEX 1300
#define WINDOWSIZEY 900

CP_Color white, black, grey;
float width, height;
int isClicked_SFX, isClicked_BGM;

void options_Init(void)
{
	white = CP_Color_Create(255, 255, 255, 255);
	black = CP_Color_Create(0, 0, 0, 255);
	grey = CP_Color_Create(100, 100, 100, 255);
	//CP_System_SetWindowSize(WINDOWSIZEX, WINDOWSIZEY);
	CP_Graphics_ClearBackground(grey);
	width = CP_System_GetWindowWidth();
	height = CP_System_GetWindowHeight();
	isClicked_SFX = 0;
	isClicked_BGM = 0;
}

void options_Update(void)
{
	CP_Graphics_ClearBackground(grey);
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	//upgrades header
	CP_Settings_TextSize(200);
	CP_Settings_Fill(white);
	CP_Font_DrawText("OPTIONS", width / 2, height / 8);
	
	char* text[] = {"SFX", "BGM" };
	CP_Settings_TextSize(50);
	for (int i = 0; i < 2; i++) 
	{
		CP_Font_DrawText(text[i], (width / 4), (height / 2) - (height / 10) + (height / 10 * i));
	}

	//SFX and BGM bar
	CP_Settings_Fill(grey);
	for (int i = 0; i < 2; i++)
	{
		CP_Graphics_DrawRectAdvanced(width / 2, (height / 2) - (height / 10) + (height / 10 * i), width / 3, height / 16, 0, 0);
	}
	//SFX slider
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_Fill(white);
	if (CP_Input_MouseClicked())
	{
		if (IsAreaClicked(width / 2, (height / 2) - (height / 10) + (height / 10 * 0), width / 3, height / 16, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
		{
			isClicked_SFX = 1;
			if (isClicked_SFX = 1)
			{
				CP_Graphics_DrawRectAdvanced(width / 2 - (width / 3 / 2), (height / 2) - (height / 10) - (height / 16 / 2), CP_Input_GetMouseX() - (width / 3), height / 16, 0, 0);
			}
		}
	}
	//BGM slider
	if (CP_Input_MouseClicked())
	{
		if (IsAreaClicked(width / 2, (height / 2) - (height / 10) + (height / 10), width / 3, height / 16, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
		{
			isClicked_BGM = 1;
			if (isClicked_BGM = 1)
			{
				CP_Graphics_DrawRectAdvanced(width / 2 - (width / 3 / 2), (height / 2) - (height / 10) + (height / 10) - (height / 16 / 2), CP_Input_GetMouseX() - (width / 3), height / 16, 0, 0);
			}
		}	
	}

	CP_Settings_RectMode(CP_POSITION_CENTER);
	//exit rectangle
	CP_Settings_Fill(black);
	CP_Graphics_DrawRectAdvanced(width / 2, height - (height / 6), width / 8, height / 8, 0, 15);
	//exit text
	CP_Settings_TextSize(50);
	CP_Settings_Fill(white);
	CP_Font_DrawText("EXIT", width / 2, height - (height / 6));

	//exit to mainmenu
	if (CP_Input_MouseClicked())
	{
		if (IsAreaClicked(width / 2, height - (height / 6), width / 8, height / 8, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}
	if (CP_Input_KeyTriggered(KEY_ESCAPE))
	{
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}
}

void options_Exit(void)
{

}
