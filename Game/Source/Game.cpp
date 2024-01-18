#include "Game.h"


Caramel::Application* CreateApplication()
{
	CL_TRACE("Creating application");
	return new MyGame();
}

class TestLayer : public Caramel::Layer
{
public:
	void OnUpdate() override
	{
		CL_INFO("Hey!");
	}

	void OnEvent(Caramel::Event& event) override
	{

	}
};

MyGame::MyGame()
{
	TestLayer* layer = new TestLayer();
	TrackLayer(layer);
}
