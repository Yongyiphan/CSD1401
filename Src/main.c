#pragma once
#include "cprocessing.h"
#include "mainmenu.h"


int main(void)
{
	CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	CP_Engine_Run();
	
	return 0;
}