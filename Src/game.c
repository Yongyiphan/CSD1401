#include "cprocessing.h"

CP_Image logo;
void game_Init(void)
{
	logo = CP_Image_Load("Assets/DigiPen_Singapore_WEB_RED.png");
	CP_Settings_ImageMode(CP_POSITION_CORNER);
	CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP);

	CP_System_SetWindowSize(CP_Image_GetWidth(logo), CP_Image_GetHeight(logo));
}

void game_Update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(0,0,0,255));
	CP_Image_Draw(logo, 0.f, 0.f, CP_Image_GetWidth(logo), CP_Image_GetHeight(logo), 255);
	if (CP_Input_KeyDown(KEY_Q))
	{
		CP_Engine_Terminate();
	}

}

void game_Exit(void)
{
	CP_Image_Free(&logo);
}
