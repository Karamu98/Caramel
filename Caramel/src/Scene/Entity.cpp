#include "clpch.h"
#include "Entity.h"

#include "Scene/Scene.h"


Caramel::Entity::~Entity()
{

}

Caramel::Entity::Entity()
{

}

Caramel::Entity::Entity(entt::entity handle, Scene* owningScene) : m_entityHandle(handle), m_scene(owningScene)
{
}
