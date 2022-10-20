#include "cprocessing.h"
#include "game.h"
#include "map.h"


int main(void)
{
	CP_Engine_SetNextGameState(game_Init, game_Update, game_Exit);
	CP_Engine_Run();
	return 0;
}