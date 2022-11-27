#include "cprocessing.h"
#include <stdio.h>
#include "audio_manager.h"
#include "options.h"
#include "mainmenu.h"


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

CP_Sound button_click = NULL;
CP_Sound pickup_exp_sound = NULL;
CP_Sound pickup_item_sound = NULL;

CP_Sound bullet_sound = NULL;

CP_Sound main_bgm_music = NULL;
CP_Sound ingame_bgm_music = NULL;

int Audioinit = 0;

void Audio_Init(void) {
	if (Audioinit == 0) {
		button_click = CP_Sound_Load("./Assets/Sound/click.wav");
		bullet_sound = CP_Sound_Load("./Assets/Sound/laserShoot.wav");
		pickup_exp_sound = CP_Sound_Load("./Assets/Sound/pickupCoin.wav");
		main_bgm_music = CP_Sound_Load("./Assets/Sound/mainmenu_music.mp3");
		ingame_bgm_music = CP_Sound_Load("./Assets/Sound/bgm_music.mp3");
		Audioinit = 1;
	}
}

void Audio_Play_Music(int sound) {
	CP_Sound_StopGroup(MUSIC);
	switch (sound) 
	{
	case Main_Menu:
		if (main_bgm_music)
			CP_Sound_PlayAdvanced(main_bgm_music, BGM_vol, 1.0f, TRUE, MUSIC);
		break;
	case In_Game:
		if (ingame_bgm_music)
			CP_Sound_PlayAdvanced(ingame_bgm_music, BGM_vol, 1.0f, TRUE, MUSIC);
		break;
	}
}

void Audio_ButtonClick(void) {
	if (button_click != NULL) {
		CP_Sound_PlayAdvanced(button_click, SFX_vol, 1.0f, FALSE, SFX);
	}
}

//Show how it aint working
void Audio_Pickup_EXP(void) {
	if (pickup_exp_sound != NULL) {
		CP_Sound_PlayAdvanced(pickup_exp_sound, SFX_vol, 1.0f, FALSE, SFX);
	}
}

void Audio_Bullet(void) {
	if (bullet_sound != NULL) {
		CP_Sound_PlayAdvanced(bullet_sound, SFX_vol, 1.0f, FALSE, SFX);
	}
}




void Audio_Exit(void) {
	if (Audioinit == 1) {
		CP_Sound_Free(&button_click);
		CP_Sound_Free(&bullet_sound);
		CP_Sound_Free(&pickup_exp_sound);

		CP_Sound_Free(&main_bgm_music);
		CP_Sound_Free(&ingame_bgm_music);
		Audioinit = 0;

	}
}