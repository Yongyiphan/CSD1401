#include "cprocessing.h"
#include "game.h"
#include "map.h"
#include "mainmenu.h"
#include <stdio.h>


CP_Sound BGM = NULL;

int main(void)
{
	/*BGM = CP_Sound_Load("./Assets/thememusic.mp3");
	CP_Sound_PlayMusic(BGM);*/
	CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	CP_Engine_Run();
	return 0;
}