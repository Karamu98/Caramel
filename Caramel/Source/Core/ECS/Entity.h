#pragma once

#include "entt/entt.hpp"
#include "Scene.h"
#include "Components/COM_Transform.h"

namespace Caramel
{
	class Entity
	{
	public:
		Entity() {};

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			return m_scene->m_scene.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			return m_scene->m_scene.get<T>(m_entityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			m_scene->m_scene.remove<T>(m_entityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_scene->m_scene.try_get<T>(m_entityHandle) != nullptr;
		}

		inline glm::vec3& RefPosition()
		{
			auto& transformComp = GetComponent<COM_Transform>();
			return transformComp.RefPosition();
		}

	private:
		friend class Scene;
		Entity(Scene* scene, entt::entity handle) : m_scene(scene), m_entityHandle(handle) {};


		Scene* m_scene;
		entt::entity m_entityHandle;
	};
}