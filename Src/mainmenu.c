	#include "cprocessing.h"
#include "utils.h"
#include "mainmenu.h"
#include "map.h"

#define WINDOWSIZEX 1300
#define WINDOWSIZEY 900

CP_Image logo;
CP_Color white, black, grey; 
float width, height;
void Main_Menu_Init()
{
	logo = CP_Image_Load(". /Assets/DigiPen_Singapore_WEB_RED.png");
	white = CP_Color_Create(255, 255, 255, 255);
	black = CP_Color_Create(0, 0, 0, 255);
	grey = CP_Color_Create(100, 100, 100, 255);
	CP_System_SetWindowSize(WINDOWSIZEX, WINDOWSIZEY);
	CP_Graphics_ClearBackground(grey);
	width = CP_System_GetWindowWidth();
	height = CP_System_GetWindowHeight();
}

void Main_Menu_Update()
{
	CP_Graphics_ClearBackground(grey);
	CP_Settings_RectMode(CP_POSITION_CENTER);

	////play rectangle
	//CP_Settings_RectMode(CP_POSITION_CENTER);
	//CP_Settings_Fill(black);
	//CP_Graphics_DrawRectAdvanced(WINDOWSIZEX / 2, (WINDOWSIZEY / 2) - 100, 200, 80, 0, 15);
	////play text
	//CP_Settings_Fill(white);
	//CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	//CP_Settings_TextSize(40.0f);
	//CP_Font_DrawText("PLAY", WINDOWSIZEX / 2, (WINDOWSIZEY / 2) - 100);

	////upgrades rectangle
	//CP_Settings_RectMode(CP_POSITION_CENTER);
	//CP_Settings_Fill(black);
	//CP_Graphics_DrawRectAdvanced(WINDOWSIZEX / 2, (WINDOWSIZEY / 2), 200, 80, 0, 15);
	////upgrades text
	//CP_Settings_Fill(white);
	//CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	//CP_Settings_TextSize(40.0f);
	//CP_Font_DrawText("UPGRADES", WINDOWSIZEX / 2, (WINDOWSIZEY / 2));

	////options rectangle
	//CP_Settings_RectMode(CP_POSITION_CENTER);
	//CP_Settings_Fill(black);
	//CP_Graphics_DrawRectAdvanced(WINDOWSIZEX / 2, (WINDOWSIZEY / 2) + 100, 200, 80, 0, 15);
	////options text
	//CP_Settings_Fill(white);
	//CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	//CP_Settings_TextSize(40.0f);
	//CP_Font_DrawText("OPTIONS", WINDOWSIZEX / 2, (WINDOWSIZEY / 2) + 100);

	////credits rectangle
	//CP_Settings_RectMode(CP_POSITION_CENTER);
	//CP_Settings_Fill(black);
	//CP_Graphics_DrawRectAdvanced(WINDOWSIZEX / 2, (WINDOWSIZEY / 2) + 200, 200, 80, 0, 15);
	////credits text
	//CP_Settings_Fill(white);
	//CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	//CP_Settings_TextSize(40.0f);
	//CP_Font_DrawText("CREDITS", WINDOWSIZEX / 2, (WINDOWSIZEY / 2) + 200);

	////exit rectangle
	//CP_Settings_RectMode(CP_POSITION_CENTER);
	//CP_Settings_Fill(black);
	//CP_Graphics_DrawRectAdvanced(WINDOWSIZEX / 2, (WINDOWSIZEY / 2) + 300, 200, 80, 0, 15);
	////exit text
	//CP_Settings_Fill(white);
	//CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	//CP_Settings_TextSize(40.0f);
	//CP_Font_DrawText("EXIT", WINDOWSIZEX / 2, (WINDOWSIZEY / 2) + 300);

	char* text[] = {"PLAY", "UPGRADES", "OPTIONS", "CREDITS", "EXIT"};
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	for (int i = 0; i < 5; i++) {
		//play rectangle
		
		CP_Settings_Fill(black);
		CP_Graphics_DrawRectAdvanced(width / 2, (height / 2) - (height / 10) + (height / 10 * i), (height / 5), ((height / 6) / 2), 0, 15);
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
		}
	}

	//click exit to exit
	if (CP_Input_MouseClicked())
	{
		if (IsAreaClicked(width / 2, (height / 2) - (height / 10) + (height / 10 * 4), (height / 5), ((height / 6) / 2), CP_Input_GetMouseX(), CP_Input_GetMouseY()))
		{
			CP_Engine_Terminate(); 
		}

	}
	if (CP_Input_KeyDown(KEY_ENTER))
	{
		CP_Engine_SetNextGameState(map_Init, map_Update, map_Exit);
	}
	if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
		CP_Engine_Terminate();
	}
}

void Main_Menu_Exit()
{

}