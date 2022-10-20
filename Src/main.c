#include "cprocessing.h"
#include "game.h"
#include "map.h"
#include "Playground.h"


int main(void)
{
	CP_Engine_SetNextGameState(game_init, game_update, game_exit);
	CP_Engine_Run();
	return 0;
}