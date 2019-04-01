#include "Light.h"
#include "glad/glad.h"
#include "glm/ext.hpp"
#include "Entity.h"
#include "Shader.h"
#include "imgui.h"

typedef Component PARENT;

Light::Light(Entity * a_pOwner) : PARENT(a_pOwner)
{
}

Light::~Light()
{
}

void Light::OnGUI()
{
}

bool Light::OnDelete()
{
	ImGui::PushID(GetOwnerEntity() + GetModelNumber());
	if (ImGui::Button("Light"))
	{
		GetOwnerEntity()->DeleteComponent(this);
		ImGui::PopID();
		return true;
	}
	else
	{
		ImGui::PopID();
		return false;
	}
	return false;
}
