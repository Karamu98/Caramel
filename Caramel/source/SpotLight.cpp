#include "SpotLight.h"
#include "Shader.h"
#include "Entity.h"
#include "Transform.h"
#include "Gizmos.h"
#include "Camera.h"

SpotLight::SpotLight(Entity * a_pOwner) : Light(a_pOwner),
m_cutoff(10.0f),
m_outerCutoff(15.0f),
m_linear(0.09f),
m_quadratic(0.032f),
m_direction(glm::vec3(0, 0, 1)),
m_attenuation(1.0f),
m_lightProjection(glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 150.0f))
{
}

SpotLight::~SpotLight()
{
}

void SpotLight::Draw(Shader * a_shader, int a_number)
{
	// Pass base here
	a_shader->SetVec3("spotLights[" + std::to_string(a_number) + "].diffuse", m_diffuseColour * m_attenuation);
	a_shader->SetVec3("spotLights[" + std::to_string(a_number) + "].specular", m_specularColour * m_attenuation);
	a_shader->SetVec2("spotLights[" + std::to_string(a_number) + "].atlasIndex", m_atlasIndex);

	// Pass spotlight here
	a_shader->SetVec3("spotLights[" + std::to_string(a_number) + "].position", GetOwnerEntity()->GetTransform()->GetPosition());
	a_shader->SetVec3("spotLights[" + std::to_string(a_number) + "].direction", m_direction);
	a_shader->SetFloat("spotLights[" + std::to_string(a_number) + "].cutOff", glm::cos(glm::radians(m_cutoff)));
	a_shader->SetFloat("spotLights[" + std::to_string(a_number) + "].outerCutOff", glm::cos(glm::radians(m_outerCutoff)));
	a_shader->SetFloat("spotLights[" + std::to_string(a_number) + "].linear", m_linear);
	a_shader->SetFloat("spotLights[" + std::to_string(a_number) + "].quadratic", m_quadratic);
	a_shader->SetMat4("spotLights[" + std::to_string(a_number) + "].projViewMatrix", m_lightProjView);
}

void SpotLight::OnGUI()
{
	if (ImGui::TreeNode("Light Component"))
	{
		ImGui::Unindent();
		Light::OnGUI();

		// Expose variables here
		ImGui::DragFloat("Inner Cutoff", &m_cutoff, 0.1f, 0.0f, m_outerCutoff);
		ImGui::DragFloat("Outter Cutoff", &m_outerCutoff, 0.1f, m_cutoff, 180.0f);
		ImGui::DragFloat("Attenuation", &m_attenuation, 0.1f, 0);
		ImGui::DragFloat3("Direction", glm::value_ptr(m_direction), 0.01f, -1.0f, 1.0f);
		ImGui::TreePop();
	}
}

void SpotLight::PrePass(Shader* a_shader, glm::vec2 a_number)
{
	// Pass light uniform
	glm::vec3 pos = GetOwnerEntity()->GetTransform()->GetPosition();

	m_lightMatrix = glm::lookAt(pos, pos + m_direction, glm::vec3(0.0f, 1.0f, 0.0f));

	m_lightProjView = m_lightProjection * m_lightMatrix;
	a_shader->SetMat4("lightSpaceMatrix", m_lightProjView);
	m_atlasIndex = a_number;
}

void SpotLight::SetDirection(glm::vec3 a_newDir)
{
	m_direction = a_newDir;
}
