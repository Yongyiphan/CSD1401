#pragma once

/* --------------------------------------
* File Level Documentation
* @author		Geoffrey Cho Jian Ming
* @email		g.cho@digipen.edu
* @contributor	Edgar Yong
* @file			audio_manager.c
* @brief		This file contains sound files
				and the loading, and freeing of music

* Copyright 2022 Digipen, All Rights Reserved.
*//*-------------------------------------*/

enum Sound_group { MUSIC, SFX };
enum Music { Main_Menu, In_Game };

void Audio_Init(void);

void Audio_ButtonClick(void);

void Audio_Pickup_EXP(void);

void Audio_Pickup_Item(void);

void Audio_Bullet(void);

void Audio_LevelUp(void);

void Audio_Play_Music(int sound);

//void Audio_MainMenu_BGM(void);

//void Audio_Stop_MainMenu_BGM(void);


void Audio_Exit(void);