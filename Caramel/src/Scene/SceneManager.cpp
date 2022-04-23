#include "clpch.h"
#include "SceneManager.h"


Caramel::SceneManager::SceneManager()
{

}

Caramel::SceneManager::~SceneManager()
{

}

void Caramel::SceneManager::Initialise(SceneManager* sceneManager)
{
	sm_instance = sceneManager;
}

void Caramel::SceneManager::Shutdown()
{
	sm_instance = nullptr;
}

Caramel::SceneManager* Caramel::SceneManager::sm_instance = nullptr;

Caramel::Scene* Caramel::SceneManager::GetActiveScene()
{
	return &sm_instance->m_activeScene;
}
