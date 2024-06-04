#pragma once

#include <entt/entt.hpp>

namespace Caramel
{
	class Entity;
	struct COM_UUID;
	struct COM_Transform;

	class Scene
	{
	public:
		Scene() {};
		~Scene() {};

		Entity CreateEntity();
		Entity CreateEntityWithUUID(UUID uuid);
		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep ts);

		Entity GetEntityByUUID(UUID uuid);

		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_scene.view<Components...>();
		}

	private:
		void RenderScene();

	private:
		entt::registry m_scene;
		std::unordered_map<UUID, entt::entity> m_entityMap;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}