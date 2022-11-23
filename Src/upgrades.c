#pragma once
#include "cprocessing.h"
#include "utils.h"
#include "mainmenu.h"
#include "upgrades.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include "items.h"

#define WINDOWSIZEX 1300
#define WINDOWSIZEY 900


CP_Color white, black, grey, dark_green, red;
CP_Image coin;
float width, height;

//upgrade health, speed, damage, fire_rate, defense, pick_up, bullet_speed;

upgrade upgrades[NUM_UPGRADES];
void upgrades_Init(void)
{
	//draw coin icon
	coin = CP_Image_Load("./Assets/Items/coin.png");
	//colors
	white = CP_Color_Create(255, 255, 255, 255);
	black = CP_Color_Create(0, 0, 0, 255);
	grey = CP_Color_Create(100, 100, 100, 255);
	dark_green = CP_Color_Create(17, 39, 0, 255);
	red = CP_Color_Create(200, 0, 0, 255);
	//window size
	//CP_System_SetWindowSize(WINDOWSIZEX, WINDOWSIZEY);
	CP_Graphics_ClearBackground(dark_green);
	width = CP_System_GetWindowWidth();
	height = CP_System_GetWindowHeight();

	//name upgrades
	char* upgradename[7] = { "HEALTH", "SPEED", "DAMAGE", "FIRE RATE", "DEFENSE", "PICK UP", "BULLET SPEED" };
	upgrades_read_from_file();
	//set upgrades to default
	for (int i = 0; i < NUM_UPGRADES; i++)
	{
		upgrades[i].name = upgradename[i];
	}
}
char* convert_int_to_string(char buffer[], int value)
{
	//char buffer[16];
	sprintf_s(buffer, _countof(buffer), "%d", value);
	return buffer;
}

void upgrades_Update(void)
{
	CP_Graphics_ClearBackground(dark_green);
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	//upgrades header
	CP_Settings_TextSize(200);
	CP_Settings_Fill(red);
	CP_Font_DrawText("UPGRADES", width / 2, height / 8);
	CP_Image_Draw(coin, width - width / 10, height / 12, width / 20, height / 16, 255);
	

	CP_Settings_Fill(black);
	//boxes for upgrades row 1
	for (int i = 0; i < 5; i++)
	{
		CP_Graphics_DrawRectAdvanced((width / 6) * (i+1), height / 3, width / 8, height / 8, 0, 15);


	}
	//draw text on boxes
	char* text[] = { "HEALTH", "SPEED", "DAMAGE", "ATTACK SPEED",  "DEFENSE" };
	//char* text = GetBaseStats(-1); //from player.c
	CP_Settings_Fill(white);
	CP_Settings_TextSize(30);
	for (int i = 0; i < 5; i++)
	{	
		char buffer[16];
		CP_Font_DrawText(text[i], (width / 6) * (i + 1), height / 3 - height / 24, width / 8);
		CP_Font_DrawText(convert_int_to_string(buffer, upgrades[i].level), (width / 6)* (i + 1), height / 3, width / 8, height / 8, 0, 15);
		//printf("string\n");
	}

	for (int i = 0; i < 5; i++)
	{
		
	}

	//draw increase rect
	for (int i = 0; i < 5; i++)
	{
		CP_Settings_Fill(grey);
		CP_Settings_NoStroke();
		CP_Graphics_DrawRectAdvanced((width / 6) * (i + 1), height / 3 + height / 10, width / 12, height / 12, 0, 10);
		if (CP_Input_MouseClicked())
		{
			if (IsAreaClicked((width / 6) * (i + 1), height / 3 + height / 10, width / 12, height / 12, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
			{
				upgrades[i].level += 1;
				printf("%d ", upgrades[i].level);
				upgrades[i].stat = upgrades[i].level * 10;
				printf("%f\n", upgrades[i].stat);
				save_all_upgrades_to_file();
			}
		}
		CP_Settings_Fill(white);
		CP_Settings_TextSize(100);
		CP_Font_DrawText("+", (width / 6) * (i + 1), height / 3 + height / 10);
	}
	//boxes for upgrades row 2
	CP_Settings_Fill(black);
	for (int i = 0; i < 5; i++)
	{
		CP_Graphics_DrawRectAdvanced((width / 6) * (i + 1), height / 3 + height / 4, width / 8, height / 8, 0, 10);
	}
	//increase rect for row 2
	for (int i = 0; i < 2; i++)
	{
		CP_Settings_Fill(grey);
		CP_Settings_NoStroke();
		CP_Graphics_DrawRectAdvanced((width / 6) * (i + 1), height / 3 + height / 4 + height / 10, width / 12, height / 12, 0, 10);
		if (CP_Input_MouseClicked())
		{
			if (IsAreaClicked((width / 6) * (i + 1), height / 3 + height / 4 + height / 10, width / 12, height / 12, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
			{
				upgrades[i+5].level += 1;
				printf("%d ", upgrades[i+5].level);
				
				upgrades[i + 5].stat = upgrades[i + 5].level * 10;

				printf("%f\n", upgrades[i+5].stat);
				save_all_upgrades_to_file();
			}
		}
		CP_Settings_Fill(white);
		CP_Settings_TextSize(100);
		CP_Font_DrawText("+", (width / 6) * (i + 1), height / 3 + height / 4 + height / 10, width / 12);
	}

	char* text2[] = { "PICK UP", "PROJECTILE SPEED" };
	CP_Settings_TextSize(30);
	CP_Settings_Fill(white);
	for (int i = 0; i < 2; i++)
	{
		char buffer[16];
		CP_Font_DrawText(text2[i], (width / 6)* (i + 1), height / 3 + height / 4 - height / 24, width / 8, height / 8);
		CP_Font_DrawText(convert_int_to_string(buffer, upgrades[i+5].level), (width / 6) * (i + 1), height / 3 + height / 4, width / 8, height / 8);
	}

	//reset button
	CP_Settings_Fill(black);
	CP_Graphics_DrawRectAdvanced((width - width / 12), height - height / 16, (width / 8), (height / 12), 0, 15);
	CP_Settings_Fill(white);
	CP_Font_DrawText("RESET", width - width / 12, height - height / 16);
	
	if (CP_Input_MouseClicked())
	{
		if (IsAreaClicked((width - width / 12), height - height / 16, (width / 8), (height / 12), CP_Input_GetMouseX(), CP_Input_GetMouseY()))
		{
			reset_all_upgrades();
			save_all_upgrades_to_file();
		}
	}

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
		{
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
			save_all_upgrades_to_file();
			CP_Settings_Stroke(black);
		}
	}
	if (CP_Input_KeyTriggered(KEY_ESCAPE)) 
	{
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update,Main_Menu_Exit);
	}

}

//read data from upgradessdata.txt
void upgrades_read_from_file(void)
{
	FILE* upgradesfile = openfile("./assets/upgradesdata.txt", "r");
	{
		if (upgradesfile)
		{
			fscanf_s(upgradesfile, "%d %f %d %f %d %f %d %f %d %f %d %f %d %f", &upgrades[0].level, &upgrades[0].stat, &upgrades[1].level, &upgrades[1].stat, &upgrades[2].level, &upgrades[2].stat, &upgrades[3].level, &upgrades[3].stat, &upgrades[4].level, &upgrades[4].stat, &upgrades[5].level, &upgrades[5].stat, &upgrades[6].level, &upgrades[6].stat);
			printf("%d %f %d %f %d %f %d %f %d %f %d %f %d %f", upgrades[0].level, upgrades[0].stat, upgrades[1].level, upgrades[1].stat, upgrades[2].level, upgrades[2].stat, upgrades[3].level, upgrades[3].stat, upgrades[4].level, upgrades[4].stat, upgrades[5].level, upgrades[5].stat, upgrades[6].level, upgrades[6].stat);
		}
	}
	closefile(upgradesfile);
}
//save data to upgradesdata.txt
void save_all_upgrades_to_file(void) 
{
	FILE* upgradesfile = openfile("./Assets/upgradesdata.txt", "r+");
	if (upgradesfile)
	{
		for (int i = 0; i < NUM_UPGRADES; i++)
		{
			fprintf(upgradesfile, "%d %f ", upgrades[i].level, upgrades[i].stat);
		}
	}
	closefile(upgradesfile);
}

//reset all upgrades
void reset_all_upgrades(void)
{
	for(int i = 0; i < NUM_UPGRADES; i++)
	{
		upgrades[i].level = 0;
		upgrades[i].cost = 0;
		upgrades[i].stat = 0;
	}
}

FILE* openfile(const char* path, const char* mode)
{
	FILE* fptr = NULL;
	errno_t err = fopen_s(&fptr, path, mode);
	if (err == 0)
	{
		printf("File %s opened.\n", path);
	}
	else
	{
		printf("File %s is not opened.\n", path);
	}
	return fptr;
}

void closefile(FILE* file)
{
	errno_t err = fclose(file);
	if (err == 0)
	{
		printf("File is closed,\n");
	}
	else
	{
		printf("File not closed.\n");
	}
}
void upgrades_Exit(void)
{

}