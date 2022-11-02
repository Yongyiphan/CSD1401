#include "cprocessing.h"
#include "game.h"
#include "map.h"
#include "mainmenu.h"

int main(void)
{
	CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	CP_Engine_Run();
	return 0;
}