#pragma once
#include "cprocessing.h"
#include "utils.h"
#include "mainmenu.h"
#include "options.h"
#include "audio_manager.h"
/* --------------------------------------
* File Level Documentation
* @author		Geoffrey Cho Jian Ming
* @email		g.cho@digipen.edu
* @file			options.c
* @brief		This file contains definition to allow player to adjust
*				BGM and SFX volum with slider bar

* Copyright 2022 Digipen, All Rights Reserved.
*//*-------------------------------------*/

#define WINDOWSIZEX 1300
#define WINDOWSIZEY 900

CP_Color white, black, grey, dark_green;
float width, height;
float SFX_length, BGM_length;
float rectWidth, rectHeight;
CP_Vector center;

void options_Init(void)
{
	white = CP_Color_Create(255, 255, 255, 255);
	black = CP_Color_Create(0, 0, 0, 255);
	grey = CP_Color_Create(100, 100, 100, 255);
	dark_green = CP_Color_Create(17, 39, 0, 255);
	CP_Graphics_ClearBackground(grey);
	width = (float) CP_System_GetWindowWidth();
	height = (float) CP_System_GetWindowHeight();
	
	rectWidth = width / 3;
	rectHeight = height / 16;
	center = CP_Vector_Set(width / 2, height / 2);
	SFX_length = rectWidth * (SFX_vol);
	BGM_length = rectWidth * BGM_vol;
	//Audio_Init();
}

void options_Update(void)
{
	CP_Graphics_ClearBackground(dark_green);
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	//upgrades header
	CP_Settings_TextSize(200);
	CP_Settings_Fill(white);
	CP_Font_DrawText("OPTIONS", center.x, height / 8);
	
	char* text[] = {"SFX", "BGM" };
	CP_Settings_TextSize(50);
	//SFX and BGM bar
	for (int i = 0; i < 2; i++) 
	{
		CP_Settings_Fill(white);
		CP_Font_DrawText(text[i], (width / 4), (center.y) - (height / 10) + (height / 10 * i));
		CP_Settings_Fill(grey);
		CP_Settings_StrokeWeight(5.0f);
		// rectangle width: rectWidth;
		CP_Graphics_DrawRectAdvanced(center.x, (center.y) - (height / 10) + (height / 10 * i), rectWidth, rectHeight, 0, 0);
	}

	
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_Fill(white);
	if (CP_Input_MouseClicked())
	{
		//SFX slider
		if (IsAreaClicked(center.x, (center.y) - (height / 10) + (height / 10 * 0), rectWidth, rectHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
		{
			SFX_length = CP_Input_GetMouseX() - (rectWidth);
			SFX_vol = SFX_length / rectWidth;
		}
		//BGM slider
		if (IsAreaClicked(center.x, (center.y) - (height / 10) + (height / 10), rectWidth, rectHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
		{
			BGM_length = CP_Input_GetMouseX() - (rectWidth);
			BGM_vol = BGM_length / rectWidth;
			CP_Sound_SetGroupVolume(MUSIC, BGM_vol);
		}
	}
	
	CP_Graphics_DrawRectAdvanced(center.x - (rectWidth / 2), (center.y) - (height / 10) - (rectHeight / 2), SFX_length, rectHeight, 0, 0);
	CP_Graphics_DrawRectAdvanced(center.x - (rectWidth / 2), (center.y) - (height / 10) + (height / 10) - (rectHeight / 2), BGM_length, rectHeight, 0, 0);


	CP_Settings_RectMode(CP_POSITION_CENTER);
	//exit rectangle
	CP_Settings_Fill(black);
	CP_Graphics_DrawRectAdvanced(center.x, height - (height / 6), width / 8, height / 8, 0, 15);
	//exit text
	CP_Settings_TextSize(50);
	CP_Settings_Fill(white);
	CP_Font_DrawText("EXIT", center.x, height - (height / 6));

	//exit to mainmenu
	if (CP_Input_MouseClicked())
	{
		if (IsAreaClicked(center.x, height - (height / 6), width / 8, height / 8, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
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