#pragma once
#include "cprocessing.h"
#include "utils.h"
#include "mainmenu.h"
#include "map.h"
#include "upgrades.h"
#include "options.h"
#include "audio_manager.h"

#define WINDOWSIZEX 1300
#define WINDOWSIZEY 900

CP_Image logo;
CP_Image title;
CP_Image zombie;
CP_Color white, black, grey, dark_green; 
CP_Sound BGM = NULL, shitSound = NULL;
float width, height;
float songLength = 500;
int isPlaying = 0;
float SFX_vol, BGM_vol;
void Main_Menu_Init(void)
{
	title = CP_Image_Load("./Assets/Title.png");
	zombie = CP_Image_Load("./Assets/Zombie.png");
	logo = CP_Image_Load(". /Assets/DigiPen_Singapore_WEB_RED.png");
	white = CP_Color_Create(255, 255, 255, 255);
	black = CP_Color_Create(0, 0, 0, 255);
	grey = CP_Color_Create(100, 100, 100, 255);
	dark_green = CP_Color_Create(17, 39, 0, 255);
	CP_System_SetWindowSize(WINDOWSIZEX, WINDOWSIZEY);
	//CP_System_Fullscreen();
	//CP_Graphics_ClearBackground(dark_green);
	width = CP_System_GetWindowWidth();
	height = CP_System_GetWindowHeight();
	Audio_Init();
	// Initialize sound values to be 70% of max volume
	SFX_vol = 0.7, BGM_vol = 0.7;
}

void Main_Menu_Update()
{
	CP_Image_Draw(title, width / 2, height / 5, width / 3, height / 4, 255);
	CP_Image_Draw(zombie, width / 2 + width / 3, height / 2, width / 4, height / 3, 255);
	CP_Image_Draw(zombie, width / 2 - width / 3, height / 2, width / 4, height / 3, 255);
	CP_Graphics_ClearBackground(dark_green);
	//CP_Graphics_ClearBackground(grey);
	CP_Settings_RectMode(CP_POSITION_CENTER);

	char* text[] = {"PLAY", "UPGRADES", "OPTIONS", "CREDITS", "EXIT"};
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	for (int i = 0; i < 5; i++) {
		//play rectangle
		
		CP_Settings_Fill(black);
		CP_Graphics_DrawRectAdvanced(width / 2, (height / 2) - (height / 10) + (height / 10 * i), (width / 5), ((height / 6) / 2), 0, 15);
		//play text
		CP_Settings_Fill(white);
		CP_Settings_TextSize(40.0f);
		CP_Font_DrawText(text[i], width / 2, (height / 2) - (height / 10) + ((height / 10) * i));
	}
	
	//click play to go to map
	if (CP_Input_MouseClicked())
	{
		if (IsAreaClicked(width / 2, (height / 2) - (height / 10) + (height / 10 * 0), (height / 5), ((height / 6) / 2), CP_Input_GetMouseX(), CP_Input_GetMouseY()))
		{
			CP_Engine_SetNextGameState(map_Init, map_Update, map_Exit);
			//CP_Sound_Free(&BGM);

		}
		//click upgrades to go to upgrades screen
		if (IsAreaClicked(width / 2, (height / 2) - (height / 10) + (height / 10 * 1), (width / 5), ((height / 6) / 2), CP_Input_GetMouseX(), CP_Input_GetMouseY()))
		{
			CP_Engine_SetNextGameState(upgrades_Init, upgrades_Update, upgrades_Exit);
		}
		//click options to go to options screen
		if (IsAreaClicked(width / 2, (height / 2) - (height / 10) + (height / 10 * 2), (width / 5), ((height / 6) / 2), CP_Input_GetMouseX(), CP_Input_GetMouseY()))
		{
			CP_Engine_SetNextGameState(options_Init, options_Update, options_Exit);
		}
		//click exit to exit
		if (IsAreaClicked(width / 2, (height / 2) - (height / 10) + (height / 10 * 4), (width / 5), ((height / 6) / 2), CP_Input_GetMouseX(), CP_Input_GetMouseY()))
		{
			CP_Engine_Terminate();
			save_all_upgrades_to_file();
		}
	}



	if (CP_Input_KeyDown(KEY_ENTER))
	{
		CP_Engine_SetNextGameState(map_Init, map_Update, map_Exit);
	}
	if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
		CP_Engine_Terminate();
	}

	if (CP_Input_KeyDown(KEY_SPACE))
	{
		Beep(400, 5);
		Beep(1000, 100);
		Beep(600, 30);
		Beep(3000, 20);
	}
	
}

void Main_Menu_Exit()
{

}