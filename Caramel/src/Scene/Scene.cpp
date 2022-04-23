#include "clpch.h"
#include "Scene.h"

#include "Entity.h"
#include "Scene/Components.h"

Caramel::Scene::Scene()
{

}

Caramel::Scene::~Scene()
{

}

Caramel::Entity Caramel::Scene::CreateEntity(std::string entityName /*= "Entity"*/)
{
	entt::entity newEntity = m_registry.create();
	m_registry.emplace<Components::EntityName>(newEntity, entityName);
	return Entity(newEntity, this);
}
