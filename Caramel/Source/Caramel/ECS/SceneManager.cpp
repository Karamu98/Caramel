#include "clpch.h"
#include "SceneManager.h"
#include "Scene.h"

namespace Caramel
{
	SceneManager* SceneManager::s_instance = nullptr;
	Ref<Scene> SceneManager::LoadScene(const void* owner, const std::string& path)
	{
		CL_CORE_NOT_IMPLEMENTED;
		return nullptr;
	}

	Ref<Scene> SceneManager::NewScene(const void* owner)
	{
		m_loadedScenes[owner] = CreateRef<Scene>();
		return m_loadedScenes[owner];
	}

	Ref<Scene> SceneManager::GetScene(const void* owner)
	{
		auto found = m_loadedScenes.find(owner);
		if (found != m_loadedScenes.end())
		{
			return (*found).second;
		}

		return nullptr;
	}

	void SceneManager::Init(SceneManager* instance)
	{
		s_instance = instance;
	}
}