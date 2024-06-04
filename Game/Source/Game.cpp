#include "Game.h"

#include <Caramel/Core/EntryPoint.h>

// TODO: This is the target API to get a game running

class TestGameLayer : public Caramel::Layer
{
public:
	virtual void OnAttach()
	{
		auto newScene = Caramel::SceneManager::Get()->LoadScene(this, "Assets/Scenes/test.scene");
		if (newScene == nullptr)
		{
			Caramel::SceneManager::Get()->NewScene(this);
		}
	}
};

MyGame::MyGame()
{
	TrackLayer(new TestGameLayer());
	EnableEditorLayer(); // TODO: strip this in release
}

Caramel::Application* Caramel::CreateApplication()
{
	CL_TRACE("Creating application");
	return new MyGame();
}
