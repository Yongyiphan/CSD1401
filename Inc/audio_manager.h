#pragma once

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

//void Audio_InGame_BGM(void);

//void Audio_Stop_InGame_BGM(void);

void Audio_Exit(void);