#include "PointLight.h"
#include "Shader.h"
#include "Entity.h"
#include "Transform.h"

PointLight::PointLight(Entity * a_pOwner) : Light(a_pOwner),
m_attenuation(1.0f),
m_constant(1.0f),
m_linear(0.09f),
m_quadratic(0.032f)
{
}

PointLight::~PointLight()
{
}

void PointLight::Update(float a_fDeltaTime)
{
}

void PointLight::Draw(Shader * a_shader, int a_number)
{
	// Pass base here
	a_shader->SetVec3("pointLights[" + std::to_string(a_number) + "].diffuse", m_diffuseColour * m_attenuation);
	a_shader->SetVec3("pointLights[" + std::to_string(a_number) + "].specular", m_specularColour * m_attenuation);

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

	ImGui::DragFloat("Attenuation", &m_attenuation, 0.1f);
}
