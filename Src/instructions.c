#include "cprocessing.h"
#include "instructions.h"
#include "items.h"
#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "utils.h"
#include "mainmenu.h"

/* --------------------------------------
* File Level Documentation
* @author	Geoffrey Cho Jian Ming
* @email	g.cho@digipen.edu
* @file		instructions.c
* @brief	This file contains instructions for the game

* Copyright 2022 Digipen, All Rights Reserved.
*//*-------------------------------------*/

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
	wWidth = (float) CP_System_GetWindowWidth();
	wHeight = (float) CP_System_GetWindowHeight();
	
	padding = 80, padUp = 10;

	startX = wWidth * 1 / 10;
	startY = wHeight * 2 / 10;
	Instructions_Image_Load();

	// variables for sub-image Base Stats
	baseSubImageCount = 6;
	baseSubImageWidth = (float) CP_Image_GetWidth(imageArr[0]) / (float) baseSubImageCount;
	baseSubImageHeight = (float) CP_Image_GetHeight(imageArr[0]);

	// variables for sub-image EXP
	expSubCount = 3;
	expSubWidth = (float) CP_Image_GetWidth(imageArr[2]) / (float) expSubCount;
	expSubHeight = (float) CP_Image_GetHeight(imageArr[2]);
}

void Instructions_Update(void) {
	CP_Graphics_ClearBackground(CP_Color_Create(200, 200, 200, 255));
	CP_Image_Draw(textArr[14], wWidth / 2.0f, wHeight * 0.1f, 
		(float) CP_Image_GetWidth(textArr[14]),(float) CP_Image_GetHeight(textArr[14]), 255);
	

	CP_Settings_Fill(CP_Color_Create(40, 40, 40, 255));
	CP_Graphics_DrawRectAdvanced(wWidth * 0.9f, wHeight * 0.1f, 200.0f, 70.0f, 0.0f, 3.0f);
	CP_Settings_Fill(CP_Color_Create(200, 200, 200, 255));
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText("BACK", wWidth * 0.9f, wHeight * 0.1f);
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) {
		if (IsAreaClicked(wWidth * 0.9f, wHeight * 0.1f, 200.0f, 70.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		}
	}
	if (CP_Input_KeyTriggered(KEY_ESCAPE))	CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);

	// Draw out images and text for base stats
	for (int i = 0; i < baseSubImageCount; i++) {
		// Draw out the images for base stats
		CP_Image_DrawSubImage(imageArr[0], startX, startY + (i * padding),
			baseSubImageWidth * 0.45f,
			baseSubImageHeight * 0.45f,
			baseSubImageWidth * i,
			0,
			baseSubImageWidth * i + baseSubImageWidth,
			baseSubImageHeight,
			255);
		// Draw out texts for base stats
		// Position the text images to the left and readjust after
		CP_Settings_ImageMode(CP_POSITION_CORNER);
		CP_Image_Draw(textArr[i], startX + 100.0f, (float) (startY - padUp + (i * padding)),
			(float) CP_Image_GetWidth(textArr[i]), (float) CP_Image_GetHeight(textArr[i]), 255);
		CP_Settings_ImageMode(CP_POSITION_CENTER);
	}

	// Draw out image and text for EXP
	float expX = wWidth * 0.5f, expY = wHeight * 0.2f;
	for (int i = 0; i < expSubCount - 1; i++) {
		CP_Image_DrawSubImage(imageArr[2], expX, expY + (i * padding),
			expSubWidth * 0.4f,								// 
			expSubHeight * 0.4f,
			expSubWidth * i,
			0,
			expSubWidth * i + expSubWidth - 50.0f,
			expSubHeight - 50.0f,
			255);
		CP_Settings_ImageMode(CP_POSITION_CORNER);
		CP_Image_Draw(textArr[i + 6], expX + 100.0f, (float)(expY - padUp + (i * padding)),
			(float) CP_Image_GetWidth(textArr[i + 6]), (float) CP_Image_GetHeight(textArr[i + 6]), 255);
		CP_Settings_ImageMode(CP_POSITION_CENTER);
	}
	
	// For Drawing Coin and Magnet Image
	CP_Image_Draw(imageArr[1], expX, expY + 2.0f * padding, (float) CP_Image_GetWidth(imageArr[1]) * 0.4f, (float) CP_Image_GetHeight(imageArr[1]) * 0.4f, 255);
	CP_Image_Draw(imageArr[4], expX, expY + 3.0f * padding, (float)CP_Image_GetWidth(imageArr[4]) * 0.4f, (float)CP_Image_GetHeight(imageArr[4]) * 0.4f, 255);
	CP_Image_Draw(textArr[10], expX + 5, expY + 4.5f * padding, (float)CP_Image_GetWidth(textArr[10]) * 0.7f, (float)CP_Image_GetHeight(textArr[10]) * 0.7f, 255);
	CP_Image_Draw(textArr[11], expX + 150, expY + 4.5f * padding, (float)CP_Image_GetWidth(textArr[11]) * 0.5f, (float)CP_Image_GetHeight(textArr[11]) * 0.5f, 255);


	CP_Settings_ImageMode(CP_POSITION_CORNER);
	// For Drawing Coin and Magnet Text
	CP_Image_Draw(textArr[8], expX + 100.0f, (expY - 3.0f * padUp) + 2.0f * padding, (float)CP_Image_GetWidth(textArr[8]), (float)CP_Image_GetHeight(textArr[8]), 255);
	CP_Image_Draw(textArr[9], expX + 100.0f, (expY - padUp) + 3.0f * padding, (float)CP_Image_GetWidth(textArr[9]), (float)CP_Image_GetHeight(textArr[9]), 255);
	CP_Image_Draw(textArr[12], expX + 220.0f, expY + 4.0f * padding, (float)CP_Image_GetWidth(textArr[12]), (float)CP_Image_GetHeight(textArr[12]), 255);

	CP_Settings_ImageMode(CP_POSITION_CENTER);
	CP_Image_Draw(textArr[13], wWidth / 2.0f, wHeight * 0.8f, (float)CP_Image_GetWidth(textArr[13]), (float)CP_Image_GetHeight(textArr[13]), 255);



	
}

void Instructions_Exit(void) {
	for (int i = 0; i < baseSubImageCount; i++) {
		CP_Image_Free(&imageArr[i]);
	}
	for (int i = 0; i < _countof(textArr); i++) {
		CP_Image_Free(&textArr[i]);
	}
}