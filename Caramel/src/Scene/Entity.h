#pragma once

#include "entt/entt.hpp"

namespace Caramel
{
	class Entity
	{
	public:
		Entity();
		~Entity();

		template<typename T, typename... Args> T& AddComponent(Args&&... args)
		{
			return m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
		}
		template<typename T> T& GetComponent()
		{
			return m_scene->m_registry.get<T>(m_entityHandle);
		}
		template<typename T> void RemoveComponent()
		{
			return m_scene->m_registry.remove<T>(m_entityHandle);
		}
		template<typename T> bool HasComponent()
		{
			return m_scene->m_registry.all_of<T>(m_entityHandle);
		}

	private:
		Entity(entt::entity handle, class Scene* owningScene);

		class Scene* m_scene;
		entt::entity m_entityHandle = entt::null;

		friend class Scene;
	};
}


