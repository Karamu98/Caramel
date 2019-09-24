#include "Game.h"

int main()
{	
	Game* newGame = new Game();
	newGame->Run("Caramel Engine", 1280, 720, false);

	delete newGame;

	return 0;
}
