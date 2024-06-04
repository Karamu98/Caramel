#pragma once

#include "Scene.h"
#include "Components/COM_Transform.h"
#include "Components/COM_UUID.h"

namespace Caramel
{
	class Entity
	{
	public:
		Entity(Scene* scene, entt::entity handle) : m_scene(scene), m_entityHandle(handle) {};
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

		operator bool() const { return m_entityHandle != entt::null; }
		operator entt::entity() const { return m_entityHandle; }
		operator uint32_t() const { return (uint32_t)m_entityHandle; }

		UUID GetUUID() { return GetComponent<COM_UUID>().Identifier; }

		bool operator==(const Entity& other) const
		{
			return m_entityHandle == other.m_entityHandle && m_scene == other.m_scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

	private:
		friend class Scene;

		Scene* m_scene = nullptr;
		entt::entity m_entityHandle { entt::null };
	};
}