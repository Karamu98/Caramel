#include "DirectionalLight.h"
#include "Shader.h"
#include "Transform.h"
#include "Entity.h"

DirectionalLight::DirectionalLight(Entity * a_pOwner) : Light(a_pOwner),
m_direction(glm::vec3(1.0f, -0.5f, 1.0)),
m_lightProjection(glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, 1.0f, 300.0f))
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
	a_shader->SetMat4("directionalLights[" + std::to_string(a_number) + "].projViewMatrix", m_lightProjView);

	// Pass directional here
	a_shader->SetVec3("directionalLights[" + std::to_string(a_number) + "].direction", m_direction);
}

void DirectionalLight::OnGUI()
{
	if (ImGui::TreeNode("Light Component"))
	{
		ImGui::Unindent();
		Light::OnGUI();

		// Expose direction here
		ImGui::DragFloat3("Direction", glm::value_ptr(m_direction), 0.05f, -1.0f, 1.0f);
		ImGui::TreePop();
	}


}

void DirectionalLight::PrePass(Shader* a_shader, glm::vec3 a_center, int a_number)
{
	// Pass light uniform
	glm::vec3 pos = a_center + glm::vec3(-m_direction.x * 50, -m_direction.y * 50, -m_direction.z * 50);// generate a position with direction

	m_lightMatrix = glm::lookAt(pos, a_center, glm::vec3(0.0f, 1.0f, 0.0f));
	m_lightProjView = m_lightProjection * m_lightMatrix;
	a_shader->SetMat4("lightSpaceMatrix", m_lightProjView);
}
