#include "cprocessing.h"
#include "game.h"
#include "map.h"

int main(void)
{
	CP_Engine_SetNextGameState(game_Init, game_Update, game_Exit);
	//CP_Engine_SetNextGameState(map_Init, map_Update, map_Exit);
	CP_Engine_Run();
	return 0;
}