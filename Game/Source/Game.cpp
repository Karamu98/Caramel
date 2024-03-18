#include "Game.h"
#include <Core/EntryPoint.h>
#include "TestGameLayer.h"

Caramel::Application* CreateApplication()
{
	CL_TRACE("Creating application");
	return new MyGame();
}

MyGame::MyGame()
{
	TestGameLayer* layer = new TestGameLayer();
	TrackLayer(layer);
}
