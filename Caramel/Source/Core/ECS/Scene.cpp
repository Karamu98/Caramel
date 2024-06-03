#include "clpch.h"
#include "Scene.h"
#include "Core/ECS/Entity.h"
#include "Components/COM_Transform.h"
#include "Components/COM_UUID.h"

namespace Caramel
{
	Entity Scene::CreateEntity()
	{
		return CreateEntityWithID(UUID());
	}

	Entity Scene::CreateEntityWithID(UUID id)
	{
		auto newEntt = Entity(this, m_scene.create());
		newEntt.AddComponent<COM_UUID>();
		newEntt.AddComponent<COM_Transform>();
		return newEntt;
	}

	void Scene::OnUpdate(Timestep ts)
	{
	}
}