#include "Light.h"
#include "glad/glad.h"
#include "glm/ext.hpp"
#include "Entity.h"
#include "Shader.h"
#include "imgui.h"

typedef Component PARENT;

Light::Light(Entity * a_pOwner) : PARENT(a_pOwner),
m_diffuseColour(glm::vec3(0.2f, 0.2f, 0.2f)),
m_specularColour(glm::vec3(0.2f, 0.2f, 0.2f))
{
}

Light::~Light()
{
}

void Light::OnGUI()
{
	ImGui::Text("Light Component");
	ImGui::NewLine();
	ImGui::InputFloat3("Diffuse Colour", glm::value_ptr(m_diffuseColour));
	ImGui::InputFloat3("Specular Colour", glm::value_ptr(m_specularColour));
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
