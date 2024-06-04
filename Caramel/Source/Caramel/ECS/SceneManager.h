#pragma once

#include "Caramel/Core/Core.h"

namespace Caramel
{
	class Scene;

	class SceneManager
	{
	public:
		Ref<Scene> LoadScene(const void* owner, const std::string& path);
		Ref<Scene> NewScene(const void* owner);
		Ref<Scene> GetScene(const void* owner);

		std::map<const void*, Ref<Scene>>::iterator begin() { return m_loadedScenes.begin(); }
		std::map<const void*, Ref<Scene>>::iterator end() { return m_loadedScenes.end(); }

	private:
		std::map<const void*, Ref<Scene>> m_loadedScenes;


	public:
		static SceneManager* Get() { return s_instance; };
	private:
		static void Init(SceneManager* instance);

	private:
		static SceneManager* s_instance;
		friend class Application;
	};
}