#include "cprocessing.h"
#include "utils.h"
#include "mainmenu.h"
#include "upgrades.h"

#define WINDOWSIZEX 1300
#define WINDOWSIZEY 900

CP_Color white, black, grey;
float width, height;

void upgrades_Init(void)
{
	white = CP_Color_Create(255, 255, 255, 255);
	black = CP_Color_Create(0, 0, 0, 255);
	grey = CP_Color_Create(100, 100, 100, 255);
	//CP_System_SetWindowSize(WINDOWSIZEX, WINDOWSIZEY);
	CP_Graphics_ClearBackground(grey);
	width = CP_System_GetWindowWidth();
	height = CP_System_GetWindowHeight();
}
void upgrades_Update(void)
{
	CP_Graphics_ClearBackground(grey);
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	//upgrades header
	CP_Settings_TextSize(200);
	CP_Settings_Fill(white);
	CP_Font_DrawText("UPGRADES", width / 2, height / 8);

	CP_Settings_Fill(black);
	//boxes for upgrades row 1
	for (int i = 0; i < 5; i++)
	{
		CP_Graphics_DrawRectAdvanced((width / 6) * (i+1), height / 3, width / 8, height / 8, 0, 15);
	}
	//draw text on boxes
	char* text[] = { "HEALTH", "SPEED", "DAMAGE", "FIRE RATE", "BULLET SPEED" };
	CP_Settings_Fill(white);
	CP_Settings_TextSize(30);
	for (int i = 0; i < 5; i++)
	{
		CP_Font_DrawText(text[i], (width / 6) * (i + 1), height / 3, width / 8);
	}

	//boxes for upgrades row 2
	CP_Settings_Fill(black);
	for (int i = 0; i < 5; i++)
	{
		CP_Graphics_DrawRectAdvanced((width / 6) * (i + 1), height / 3 + height / 4, width / 8, height / 8, 0, 15);
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
		if (IsAreaClicked(width / 2, height - (height / 6), width / 8, height / 8, CP_Input_GetMouseX(),CP_Input_GetMouseY()))
			CP_Engine_SetNextGameState( Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}
	if (CP_Input_KeyTriggered(KEY_ESCAPE)) 
	{
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update,Main_Menu_Exit);
	}

}
void upgrades_Exit(void)
{

}