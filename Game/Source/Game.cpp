#include "Game.h"
Caramel::Application* CreateApplication()
{
	CL_TRACE("Creating application");
	return new MyGame();
}



void MyGame::Run()
{
	CL_TRACE("Application begin run");
	while (true)
	{
		//CL_TRACE("Hey");
	}
}


