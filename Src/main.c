#pragma once
#include "cprocessing.h"
#include "mainmenu.h"
#include <stdio.h>

/* --------------------------------------
* File Level Documentation
* @contributor	Sen Chuan Tay, Michael Mah, Edgar Yong, Geoffrey Cho
* @file			main.c
* @brief		This File is the entry point of the game

* Copyright 2022 Digipen, All Rights Reserved.
*//*-------------------------------------*/

int main(void)
{
	CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	CP_Engine_Run();
	return 0;
}