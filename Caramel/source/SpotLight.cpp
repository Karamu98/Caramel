#include "SpotLight.h"
#include "Shader.h"
#include "Entity.h"
#include "Transform.h"

SpotLight::SpotLight(Entity * a_pOwner) : Light(a_pOwner),
m_cutoff(3),
m_outerCutoff(4),
m_constant(1.0f),
m_linear(0.7f),
m_quadratic(1.8f)
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
	a_shader->SetVec3("spotLights[" + std::to_string(a_number) + "].position", GetOwnerEntity()->GetTransform()->GetPosition());
	a_shader->SetVec3("spotLights[" + std::to_string(a_number) + "].direction", m_direction);
	a_shader->SetFloat("spotLights[" + std::to_string(a_number) + "].cutoff", m_cutoff);
	a_shader->SetFloat("spotLights[" + std::to_string(a_number) + "].outerCutoff", m_outerCutoff);
	a_shader->SetFloat("spotLights[" + std::to_string(a_number) + "].constant", m_constant);
	a_shader->SetFloat("spotLights[" + std::to_string(a_number) + "].linear", m_linear);
	a_shader->SetFloat("spotLights[" + std::to_string(a_number) + "].quadratic", m_quadratic);
}

void SpotLight::OnGUI()
{
	Light::OnGUI();

	// Expose variables here
}
