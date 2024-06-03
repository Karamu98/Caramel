#pragma once

#include <entt/entt.hpp>

namespace Caramel
{
	class Entity;

	class Scene
	{
	public:
		void Save(const std::string& path);
		void Load(const std::string& path, bool additive = false);

		Entity CreateEntity();
		Entity CreateEntityWithID(UUID id);

		void OnUpdate(Timestep ts);

	private:
		friend class Entity;
		entt::registry m_scene;
	};
}