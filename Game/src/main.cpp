#include "Game.h"

int main()
{	
	Game* newGame = new Game();
	newGame->Run("Caramel Engine", 1920, 1080, false);

	delete newGame;

	return 0;
}
