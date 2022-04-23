#pragma once

#include "entt/entt.hpp"
#include "Core/Asset.h"

#include "Entity.h"

namespace Caramel
{
	class Scene : public Asset
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(std::string entityName = "Entity");

	private:
		friend class SceneManager;
		friend class Entity;

		entt::registry m_registry;
	};
}
