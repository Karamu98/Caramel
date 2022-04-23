#include "Game.h"

int main()
{
	Game newGame;
	Caramel::Caramel engine(&newGame);
	engine.Run("Caramel Test", 1920, 1080, false);

	return 0;
}
