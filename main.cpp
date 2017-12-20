#include "snake_game.h"
#include "utils.h"

int main()
{
	HideConsoleCursor();
	srand(time(NULL));
	Game game;
	game.generateGamePlace(HEIGHT, WIDTH);
	game.start();
	return 0;
}
