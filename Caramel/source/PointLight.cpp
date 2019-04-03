#include "PointLight.h"
#include "Shader.h"
#include "Entity.h"
#include "Transform.h"

PointLight::PointLight(Entity * a_pOwner) : Light(a_pOwner),
m_constant(1.0f),
m_linear(0.7f),
m_quadratic(1.8f)
{
}

PointLight::~PointLight()
{
}

void PointLight::Draw(Shader * a_shader, int a_number)
{
	// Pass base here
	a_shader->SetVec3("pointLights[" + std::to_string(a_number) + "].ambient", m_ambientColour);
	a_shader->SetVec3("pointLights[" + std::to_string(a_number) + "].diffuse", m_diffuseColour);
	a_shader->SetVec3("pointLights[" + std::to_string(a_number) + "].specular", m_specularColour);

	// Pass pointlight here
	a_shader->SetVec3("pointLights[" + std::to_string(a_number) + "].position", GetOwnerEntity()->GetTransform()->GetPosition());
	a_shader->SetFloat("pointLights[" + std::to_string(a_number) + "].constant", m_constant);
	a_shader->SetFloat("pointLights[" + std::to_string(a_number) + "].linear", m_linear);
	a_shader->SetFloat("pointLights[" + std::to_string(a_number) + "].quadratic", m_quadratic);
}

void PointLight::OnGUI()
{
	Light::OnGUI();
	// Expose colours and values from base and pointlight here
}