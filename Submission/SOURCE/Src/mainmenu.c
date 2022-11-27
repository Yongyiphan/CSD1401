#pragma once
#include "cprocessing.h"
#include "utils.h"
#include "mainmenu.h"
#include "map.h"
#include "upgrades.h"
#include "options.h"
#include "audio_manager.h"
#include "credits.h"
#include "instructions.h"
#include <math.h>

/* --------------------------------------
* File Level Documentation
* @author:		Mah Ming Qian Michael
* @email:		m.mah@digipen.edu
* @contributor	Geoffrey Cho, Edgar Yong
* @file:		mainmenu.c
* @brief:		Main menu source file.
				Manages initialization, updating and drawing of main menu.
				Contains functions to display other sub menus to upgrades,
				options, instructions, credits.
				Entry point into the main game and also the exit.

* Copyright 2022 Digipen, All rights reserved
*//*-------------------------------------*/




#define WINDOWSIZEX 1300
#define WINDOWSIZEY 900

CP_Image logo;
CP_Image title;
CP_Image zombie;
CP_Color white, black, grey, dark_green, red; 
CP_Sound BGM = NULL, shitSound = NULL;
float width, height;
float songLength = 500;
int isPlaying = 0;
int MMAudio = 0;
float SFX_vol = 0.8f, BGM_vol = 0.8f;
//float SFX_vol = 0.5f, BGM_vol = 0.05f;
int splashscreen_time = 2;
void Main_Menu_Init(void)
{
	title = CP_Image_Load("./Assets/Title.png");
	zombie = CP_Image_Load("./Assets/Zombie.png");
	logo = CP_Image_Load("./Assets/DigiPen_Singapore_WEB_WHITE.png");
	white = CP_Color_Create(255, 255, 255, 255);
	black = CP_Color_Create(0, 0, 0, 255);
	grey = CP_Color_Create(100, 100, 100, 255);
	dark_green = CP_Color_Create(17, 39, 0, 255);
	red = CP_Color_Create(200, 0, 0, 255);
	CP_System_SetWindowSize(WINDOWSIZEX, WINDOWSIZEY);
	width = (float) CP_System_GetWindowWidth();
	height = (float) CP_System_GetWindowHeight();
	
	Audio_Init();
	MMAudio = 0;
}


void Main_Menu_Update()
{
	CP_Graphics_ClearBackground(black);
	float currentElapsedTime = CP_System_GetDt();
	static float totalElapsedTime = 0;
	totalElapsedTime += currentElapsedTime;
	int transparency = 256 - abs((int)(totalElapsedTime / splashscreen_time * 256) % 512 - 256);
	if (totalElapsedTime < splashscreen_time)
	{
		CP_Image_Draw(logo, (float) CP_System_GetWindowWidth() / 2,(float) CP_System_GetWindowHeight() / 2.0f, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight()/2.0f, transparency);
	}
	if (totalElapsedTime >= splashscreen_time)
	{
		if (MMAudio == 0) {
			Audio_Play_Music(Main_Menu);
			MMAudio = 1;
		}
		CP_Graphics_ClearBackground(dark_green);
		CP_Image_Draw(title, width / 2.0f, height / 5.0f, width / 3.0f, height / 4.0f, 255);
		CP_Image_Draw(zombie, width / 2.0f + width / 3.0f, height / 2.0f, width / 4.0f, height / 3.0f, 255);
		CP_Image_Draw(zombie, width / 2.0f - width / 3.0f, height / 2.0f, width / 4.0f, height / 3.0f, 255);

		CP_Settings_RectMode(CP_POSITION_CENTER);

		char* text[] = {"PLAY", "UPGRADES", "HOW TO PLAY", "OPTIONS", "CREDITS", "EXIT"};
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		for (int i = 0; i < 6; i++) {
			//play rectangle

			CP_Settings_Fill(black);
			CP_Graphics_DrawRectAdvanced(width / 2.0f, (height / 2.0f) - (height / 10.0f) + (height / 10.0f * i), (width / 5.0f), ((height / 6.0f) / 2.0f), 0, 15.0f);
			//play text
			CP_Settings_Fill(red);
			CP_Settings_TextSize(40.0f);
			CP_Font_DrawText(text[i], width / 2.0f, (height / 2.0f) - (height / 10.0f) + ((height / 10.0f) * i));
		}

		//click play to go to map
		if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			
			if (IsAreaClicked(width / 2.0f, (height / 2.0f) - (height / 10.0f) + (height / 10.0f * 0.0f), (height / 5.0f), ((height / 6.0f) / 2.0f), CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
				CP_Engine_SetNextGameState(map_Init, map_Update, map_Exit);
			}
			//click upgrades to go to upgrades screen
			if (IsAreaClicked(width / 2.0f, (height / 2.0f) - (height / 10.0f) + (height / 10.0f * 1.0f), (height / 5.0f), ((height / 6.0f) / 2.0f), CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
				CP_Engine_SetNextGameState(upgrades_Init, upgrades_Update, upgrades_Exit);
			}
			//click how to play to go to instructions screen
			if (IsAreaClicked(width / 2.0f, (height / 2.0f) - (height / 10.0f) + (height / 10.0f * 2.0f), (height / 5.0f), ((height / 6.0f) / 2.0f), CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
				CP_Engine_SetNextGameState(Instructions_Init, Instructions_Update, Instructions_Exit);
			}
			
			//click options to go to options screen
			if (IsAreaClicked(width / 2.0f, (height / 2.0f) - (height / 10.0f) + (height / 10.0f * 3.0f), (height / 5.0f), ((height / 6.0f) / 2.0f), CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
				CP_Engine_SetNextGameState(options_Init, options_Update, options_Exit);
			}
			if (IsAreaClicked(width / 2.0f, (height / 2.0f) - (height / 10.0f) + (height / 10.0f * 4.0f), (height / 5.0f), ((height / 6.0f) / 2.0f), CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
				CP_Engine_SetNextGameState(Credits_Init, Credits_Update, Credits_Exit);
			}
			//click exit to exit
			if (IsAreaClicked(width / 2.0f, (height / 2.0f) - (height / 10.0f) + (height / 10.0f * 5.0f), (height / 5.0f), ((height / 6.0f) / 2.0f), CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
				save_all_upgrades_to_file();
				Audio_Exit();
				CP_Engine_Terminate();
			}
		}
	}

}


void Main_Menu_Exit()
{
	//Audio_Exit();
}