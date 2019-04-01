#include "Light.h"
#include "glad/glad.h"
#include "glm/ext.hpp"
#include "Entity.h"
#include "Shader.h"
#include "imgui.h"

typedef Component PARENT;

Light::Light(Entity * a_pOwner) : PARENT(a_pOwner),
m_ambientColour(glm::vec3(0.01f, 1, 0.05f)),
m_diffuseColour(glm::vec3(0.4f, 0.9f, 0.4f)),
m_specularColour(glm::vec3(0.5f, 0.5f, 0.5f))
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
