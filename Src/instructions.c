#include "cprocessing.h"
#include "instructions.h"
#include "items.h"
#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "utils.h"
#include "mainmenu.h"


CP_Image imageArr[20], textArr[20];
float wWidth, wHeight;
float startX, startY;
int baseSubImageCount, expSubCount;
float baseSubImageWidth, baseSubImageHeight, expSubWidth, expSubHeight;
float padding, padUp;

void Instructions_Image_Load(void) {
	
	char* path[] = {
		"./Assets/Items/Base Item Sprite.png",
		"./Assets/Items/coin.png",
		"./Assets/Items/EXP Sprite.png",
		"./Assets/Items/GreenExp",
		"./Assets/Items/Magnet.png",
		"./Assets/Items/placeholderbullet.png"
	};

	char* text[] = {
		"./Assets/Instructions/hp_text.png",
		"./Assets/Instructions/mvmtSpd_text.png",
		"./Assets/Instructions/dmg_text.png",
		"./Assets/Instructions/atkSpd_text.png",
		"./Assets/Instructions/bulletSpd_text.png",
		"./Assets/Instructions/maxHP_text.png",
		"./Assets/Instructions/normal_exp.png",
		"./Assets/Instructions/great_exp.png",
		"./Assets/Instructions/coin_text.png",
		"./Assets/Instructions/magnet_text.png",
		"./Assets/Instructions/wasdKey.png",
		"./Assets/Instructions/mouse.png",
		"./Assets/Instructions/directionalKeys_text.png",
		"./Assets/Instructions/warning_text.png",
		"./Assets/Instructions/instruct_text.png"
	};

	// Load images for temporary base stat increases
	for (int i = 0; i < _countof(path); i++) {
		imageArr[i] = CP_Image_Load(path[i]);
	}
	// Load texts
	for (int i = 0; i < _countof(text); i++) {
		textArr[i] = CP_Image_Load(text[i]);
	}
}

void Instructions_Init(void) {
	CP_System_SetWindowSize(1300, 900);
	wWidth = CP_System_GetWindowWidth();
	wHeight = CP_System_GetWindowHeight();
	
	padding = 80, padUp = 10;

	startX = wWidth * 1 / 10;
	startY = wHeight * 2 / 10;
	Instructions_Image_Load();

	// variables for sub-image Base Stats
	baseSubImageCount = 6;
	baseSubImageWidth = CP_Image_GetWidth(imageArr[0]) / (float) baseSubImageCount;
	baseSubImageHeight = CP_Image_GetHeight(imageArr[0]);

	// variables for sub-image EXP
	expSubCount = 3;
	expSubWidth = CP_Image_GetWidth(imageArr[2]) / (float) expSubCount;
	expSubHeight = CP_Image_GetHeight(imageArr[2]);
}

void Instructions_Update(void) {
	CP_Graphics_ClearBackground(CP_Color_Create(200, 200, 200, 255));
	CP_Image_Draw(textArr[14], wWidth / 2, wHeight * 0.1, CP_Image_GetWidth(textArr[14]), CP_Image_GetHeight(textArr[14]), 255);
	

	CP_Settings_Fill(CP_Color_Create(40, 40, 40, 255));
	CP_Graphics_DrawRectAdvanced(wWidth * 0.9, wHeight * 0.1, 200, 70, 0, 3);
	CP_Settings_Fill(CP_Color_Create(200, 200, 200, 255));
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText("BACK", wWidth * 0.9, wHeight * 0.1);
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
		if (IsAreaClicked(wWidth * 0.9, wHeight * 0.1, 200, 70, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		}
	}
	if (CP_Input_KeyTriggered(KEY_ESCAPE))	CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);

	// Draw out images and text for base stats
	for (int i = 0; i < baseSubImageCount; i++) {
		// Draw out the images for base stats
		CP_Image_DrawSubImage(imageArr[0], startX, startY + (i * padding),
			baseSubImageWidth * 0.45,
			baseSubImageHeight * 0.45,
			baseSubImageWidth * i,
			0,
			baseSubImageWidth * i + baseSubImageWidth,
			baseSubImageHeight,
			255);
		// Draw out texts for base stats
		// Position the text images to the left and readjust after
		CP_Settings_ImageMode(CP_POSITION_CORNER);
		CP_Image_Draw(textArr[i], startX + 100, startY - padUp + (i * padding),
			CP_Image_GetWidth(textArr[i]), CP_Image_GetHeight(textArr[i]), 255);
		CP_Settings_ImageMode(CP_POSITION_CENTER);
	}

	// Draw out image and text for EXP
	float expX = wWidth * 0.5, expY = wHeight * 0.2;
	for (int i = 0; i < expSubCount - 1; i++) {
		CP_Image_DrawSubImage(imageArr[2], expX, expY + (i * padding),
			expSubWidth * 0.4,								// 
			expSubHeight * 0.4,
			expSubWidth * i,
			0,
			expSubWidth * i + expSubWidth - 50,
			expSubHeight - 50,
			255);
		CP_Settings_ImageMode(CP_POSITION_CORNER);
		CP_Image_Draw(textArr[i + 6], expX + 100, expY - padUp + (i * padding),
			CP_Image_GetWidth(textArr[i + 6]), CP_Image_GetHeight(textArr[i + 6]), 255);
		CP_Settings_ImageMode(CP_POSITION_CENTER);
	}
	
	// For Drawing Coin and Magnet Image
	CP_Image_Draw(imageArr[1], expX, expY + 2 * padding, CP_Image_GetWidth(imageArr[1]) * 0.4, CP_Image_GetHeight(imageArr[1]) * 0.4, 255);
	CP_Image_Draw(imageArr[4], expX, expY + 3 * padding, CP_Image_GetWidth(imageArr[4]) * 0.4, CP_Image_GetHeight(imageArr[4]) * 0.4, 255);
	CP_Image_Draw(textArr[10], expX + 5, expY + 4.5 * padding, CP_Image_GetWidth(textArr[10]) * 0.7, CP_Image_GetHeight(textArr[10]) * 0.7, 255);
	CP_Image_Draw(textArr[11], expX + 150, expY + 4.5 * padding, CP_Image_GetWidth(textArr[11]) * 0.5, CP_Image_GetHeight(textArr[11]) * 0.5, 255);


	CP_Settings_ImageMode(CP_POSITION_CORNER);
	// For Drawing Coin and Magnet Text
	CP_Image_Draw(textArr[8], expX + 100, (expY - 3 * padUp) + 2 * padding, CP_Image_GetWidth(textArr[8]), CP_Image_GetHeight(textArr[8]), 255);
	CP_Image_Draw(textArr[9], expX + 100, (expY - padUp) + 3 * padding, CP_Image_GetWidth(textArr[9]), CP_Image_GetHeight(textArr[9]), 255);
	CP_Image_Draw(textArr[12], expX + 220, expY + 4 * padding, CP_Image_GetWidth(textArr[12]), CP_Image_GetHeight(textArr[12]), 255);

	CP_Settings_ImageMode(CP_POSITION_CENTER);
	CP_Image_Draw(textArr[13], wWidth / 2, wHeight * 0.8, CP_Image_GetWidth(textArr[13]), CP_Image_GetHeight(textArr[13]), 255);



	
}

void Instructions_Exit(void) {
	for (int i = 0; i < baseSubImageCount; i++) {
		CP_Image_Free(imageArr[i]);
	}
	for (int i = 0; i < _countof(textArr); i++) {
		CP_Image_Free(textArr[i]);
	}
}