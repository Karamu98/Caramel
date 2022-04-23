#pragma once

#include "entt/entt.hpp"
#include "Scene.h"

namespace Caramel
{
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		
	private:
		std::vector<std::shared_ptr<Scene>> m_activeScenes;
		Scene m_activeScene;

	public:
		static void Initialise(SceneManager* sceneManager);
		static void Shutdown();
		static Scene* GetActiveScene();

	private:
		static SceneManager* sm_instance;
	};
}