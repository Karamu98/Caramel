#include "clpch.h"
#include "Scene.h"
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
		return newEntt;
	}

	void Scene::OnUpdate(Timestep ts)
	{
	}
}