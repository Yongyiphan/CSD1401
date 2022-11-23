#include "audio_manager.h"
#include "options.h"
#include "cprocessing.h"
#include "mainmenu.h"


CP_Sound button_click;
CP_Sound pickup_exp_sound;
CP_Sound pickup_item_sound;
CP_Sound bullet_sound;
CP_Sound level_up_sound;
CP_Sound main_bgm_music;
CP_Sound ingame_bgm_music;




void Audio_Init(void) {
	button_click = CP_Sound_Load("./Assets/Sound/click.wav");
	pickup_exp_sound = CP_Sound_Load("./Assets/Sound/pickupCoin.wav");
	pickup_item_sound = CP_Sound_Load("./Assets/Sound/powerUp.wav");
	bullet_sound = CP_Sound_Load("./Assets/Sound/laserShoot.wav");
	level_up_sound = CP_Sound_Load("./Assets/Sound/levelUp.wav");
	main_bgm_music = CP_Sound_Load("./Assets/Sound/mainmenuBGM.mp3");
	ingame_bgm_music = CP_Sound_Load("./Assets/Sound/ingameBGM.mp3");
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
	CP_Sound_PlayAdvanced(button_click, SFX_vol, 1.0f, FALSE, SFX);
}

void Audio_Pickup_EXP(void) {
	CP_Sound_PlayAdvanced(pickup_exp_sound, SFX_vol, 1.0f, FALSE, SFX);
}

void Audio_Pickup_Item(void) {
	CP_Sound_PlayAdvanced(button_click, SFX_vol, 1.0f, FALSE, SFX);
}

void Audio_Bullet(void) {
	CP_Sound_PlayAdvanced(bullet_sound, SFX_vol, 1.0f, FALSE, SFX);
}

void Audio_LevelUp(void){
	CP_Sound_PlayAdvanced(level_up_sound, SFX_vol, 1.0f, FALSE, SFX);
}

void Audio_MainMenu_BGM(void) {
	static int isPlayed = 0;

	if (isPlayed == 0) {
		CP_Sound_PlayAdvanced(main_bgm_music, BGM_vol, 1.0f, TRUE, MUSIC);
		isPlayed = 1;
	}
	
}

void Audio_Stop_MainMenu_BGM(void) {
	CP_Sound_StopGroup(MUSIC);
}

void Audio_Stop_InGame_BGM(void) {
	CP_Sound_StopGroup(MUSIC);
}

void Audio_InGame_BGM(void) {
	CP_Sound_PlayAdvanced(ingame_bgm_music, BGM_vol, 1.0f, TRUE, MUSIC);
}


void Audio_Exit(void) {
	CP_Sound_Free(button_click);
	CP_Sound_Free(pickup_exp_sound);
	CP_Sound_Free(pickup_item_sound);
	CP_Sound_Free(bullet_sound);
	CP_Sound_Free(level_up_sound);
	CP_Sound_Free(pickup_exp_sound);
	CP_Sound_Free(main_bgm_music);
	CP_Sound_Free(ingame_bgm_music);
}