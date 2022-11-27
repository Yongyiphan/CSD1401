/*----------------------------------------------------------------------
	file:	mainmenu.h
  author:	Mah Ming Qian Michael
   email:	m.mah@digipen.edu

   brief:	Main menu header file.
				Manages initialization, updating and drawing of main menu.
				Contains functions to display other sub menus to upgrades,
				options, instructions, credits.
				Entry point into the main game and also the exit.

Copyright 2022 Digipen, All rights reserved
----------------------------------------------------------------------*/
#pragma once
#ifndef MAINMENU_H
#define MAINMENU_H

void Main_Menu_Init(void);
void Main_Menu_Update(void);
void Main_Menu_Exit(void);

extern float SFX_vol, BGM_vol;

#endif