#include "DirectionalLight.h"
#include "Shader.h"
#include "Transform.h"


DirectionalLight::DirectionalLight(Entity * a_pOwner) : Light(a_pOwner),
m_direction(glm::vec3(1.0f, -0.5f, 1.0))
{
	
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::Draw(Shader* a_shader, int a_number)
{
	// Pass base here
	a_shader->SetVec3("directionalLights[" + std::to_string(a_number) + "].diffuse", m_diffuseColour);
	a_shader->SetVec3("directionalLights[" + std::to_string(a_number) + "].specular", m_specularColour);

	// Pass directional here
	a_shader->SetVec3("directionalLights[" + std::to_string(a_number) + "].direction", m_direction);
}

void DirectionalLight::OnGUI()
{
	Light::OnGUI();

	// Expose direction here
	ImGui::InputFloat3("Direction", glm::value_ptr(m_direction));
}
