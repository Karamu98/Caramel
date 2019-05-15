#include "Light.h"
#include "glad/glad.h"
#include "glm/ext.hpp"
#include "Entity.h"
#include "Shader.h"
#include "imgui.h"

typedef Component PARENT;

Light::Light(Entity * a_pOwner) : PARENT(a_pOwner),
m_diffuseColour(glm::vec3(0.8f, 0.8f, 0.8f)),
m_specularColour(glm::vec3(1.0f, 1.0f, 1.0f))
{
}

Light::~Light()
{
}

void Light::OnGUI()
{
	ImGui::ColorEdit3("Diffuse", glm::value_ptr(m_diffuseColour));
	ImGui::ColorEdit3("Specular", glm::value_ptr(m_specularColour));
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

void Light::SetDiffuse(glm::vec3 a_colour)
{
	m_diffuseColour = a_colour;
}

void Light::SetSpecular(glm::vec3 a_spec)
{
	m_specularColour = a_spec;
}
