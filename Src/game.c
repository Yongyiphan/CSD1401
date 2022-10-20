#include "cprocessing.h"
#include "map.h"

#define MAP_SIZE 700

CP_Image logo;
void game_Init(void)
{
	
	CP_System_Fullscreen();
}

void game_Update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100,255));
	//CP_Image_Draw(logo, 0.f, 0.f, CP_Image_GetWidth(logo), CP_Image_GetHeight(logo), 255);
	if (CP_Input_KeyDown(KEY_ENTER))
	{
		CP_Engine_SetNextGameState(map_Init, map_Update, map_Exit);
	}
	if (CP_Input_KeyTriggered(KEY_ESCAPE))
		CP_Engine_Terminate();

}

void game_Exit(void)
{
	CP_Image_Free(&logo);
}
