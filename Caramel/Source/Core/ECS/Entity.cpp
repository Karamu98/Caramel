#include "clpch.h"
#include "Entity.h"
#include "Components/COM_Transform.h"
#include "Scene.h"

namespace Caramel
{
	glm::vec3& Entity::RefPosition()
	{
		auto& transformComp = GetComponent<COM_Transform>();
		return transformComp.RefPosition();
	}
}