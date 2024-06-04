#include "clpch.h"
#include "Scene.h"
#include "Caramel/ECS/Entity.h"

#include "Components/COM_UUID.h"
#include "Components/COM_Transform.h"
#include "Components/COM_Model.h"

#include "Caramel/Renderer/Renderer.h"

namespace Caramel
{
	Entity Caramel::Scene::CreateEntity()
	{
		return CreateEntityWithUUID(UUID());
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid)
	{
		Entity newEntity = { this, m_scene.create() };
		newEntity.AddComponent<COM_UUID>(uuid);
		newEntity.AddComponent<COM_Transform>();

		m_entityMap[uuid] = newEntity;
		return newEntity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_entityMap.erase(entity.GetUUID());
		m_scene.destroy(entity);
	}

	void Scene::OnUpdate(Timestep ts)
	{
		RenderScene();
	}

	Entity Scene::GetEntityByUUID(UUID uuid)
	{
		if (m_entityMap.find(uuid) != m_entityMap.end())
		{
			return { this, m_entityMap.at(uuid)};
		}

		return {};
	}

	void Scene::RenderScene()
	{
		Renderer::BeginScene();

		// Render models
		{
			auto view = m_scene.view<COM_Transform, COM_Model>();
			for (auto entity : view)
			{
				auto [transform, model] = view.get<COM_Transform, COM_Model>(entity);

				Renderer::Submit(model.Model, transform.Transform);
			}
		}

		Renderer::EndScene();
	}
}