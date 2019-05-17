#include "SpotLight.h"
#include "Shader.h"
#include "Entity.h"
#include "Transform.h"
#include "Gizmos.h"
#include "Camera.h"
#include "Defines.h"

SpotLight::SpotLight(Entity * a_pOwner) : Light(a_pOwner),
m_cutoff(10.0f),
m_outerCutoff(15.0f),
m_linear(0.09f),
m_quadratic(0.032f),
m_direction(glm::vec3(0, 0, 1)),
m_attenuation(1.0f)
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
		ImGui::Indent();
	}
}

Component* SpotLight::Duplicate(Entity* a_owner)
{
	SpotLight* newCopy = new SpotLight(a_owner);

	newCopy->m_diffuseColour = this->m_diffuseColour;
	newCopy->m_specularColour = this->m_specularColour;

	newCopy->m_attenuation = this->m_attenuation;
	newCopy->m_linear = this->m_linear;
	newCopy->m_quadratic = this->m_quadratic;
	newCopy->m_atlasIndex = this->m_atlasIndex;
	newCopy->m_cutoff = this->m_cutoff;
	newCopy->m_outerCutoff = this->m_outerCutoff;
	newCopy->m_direction = this->m_direction;

	return newCopy;
}

void SpotLight::Save(std::ostream* a_outStream)
{
	FileHeader sptHead;
	sptHead.flag = Flags::SPTLIGHT_START;
	sptHead.size = sizeof(SpotLight);
	SaveToFile(a_outStream, sptHead);

	SaveToFile(a_outStream, m_diffuseColour);
	SaveToFile(a_outStream, m_specularColour);
	SaveToFile(a_outStream, m_cutoff);
	SaveToFile(a_outStream, m_outerCutoff);
	SaveToFile(a_outStream, m_linear);
	SaveToFile(a_outStream, m_quadratic);
	SaveToFile(a_outStream, m_attenuation);
}

void SpotLight::PrePass(Shader* a_shader, glm::vec2 a_number)
{

}

void SpotLight::SetDirection(glm::vec3 a_newDir)
{
	m_direction = a_newDir;
}
