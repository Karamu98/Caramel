#include "SpotLight.h"
#include "Shader.h"
#include "Entity.h"
#include "Transform.h"
#include "Gizmos.h"
#include "Camera.h"

SpotLight::SpotLight(Entity * a_pOwner) : Light(a_pOwner),
m_cutoff(10.0f),
m_outerCutoff(15.0f),
m_constant(1.0f),
m_linear(0.09f),
m_quadratic(0.032f),
m_direction(glm::vec3(0, 0, 1))
{
}

SpotLight::~SpotLight()
{
}

void SpotLight::Draw(Shader * a_shader, int a_number)
{
	// Pass base here
	a_shader->SetVec3("spotLights[" + std::to_string(a_number) + "].diffuse", m_diffuseColour);
	a_shader->SetVec3("spotLights[" + std::to_string(a_number) + "].specular", m_specularColour);

	// Pass spotlight here
	a_shader->SetVec3("spotLights[" + std::to_string(a_number) + "].position", glm::vec4(GetOwnerEntity()->GetComponentOfType<Camera>()->GetCameraMatrix()[3]));
	a_shader->SetVec3("spotLights[" + std::to_string(a_number) + "].direction", m_direction);
	a_shader->SetFloat("spotLights[" + std::to_string(a_number) + "].cutOff", glm::cos(glm::radians(m_cutoff)));
	a_shader->SetFloat("spotLights[" + std::to_string(a_number) + "].outerCutOff", glm::cos(glm::radians(m_outerCutoff)));
	a_shader->SetFloat("spotLights[" + std::to_string(a_number) + "].constant", m_constant);
	a_shader->SetFloat("spotLights[" + std::to_string(a_number) + "].linear", m_linear);
	a_shader->SetFloat("spotLights[" + std::to_string(a_number) + "].quadratic", m_quadratic);
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
		ImGui::InputFloat3("Direction", glm::value_ptr(m_direction));
		ImGui::TreePop();
	}
}

void SpotLight::SetDirection(glm::vec3 a_newDir)
{
	m_direction = a_newDir;
}
