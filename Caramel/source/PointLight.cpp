#include "PointLight.h"
#include "Shader.h"
#include "Entity.h"
#include "Transform.h"
#include "Cube.h"

PointLight::PointLight(Entity * a_pOwner) : Light(a_pOwner),
m_attenuation(1.0f),
m_linear(0.09f),
m_quadratic(0.032f)
{
	m_cubeRepresentation = new Cube();
	m_lightTransform = Transform();

	m_lightTransform.SetScale(glm::vec3(0.1f));
}

PointLight::~PointLight()
{
	delete m_cubeRepresentation;
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
	a_shader->SetFloat("pointLights[" + std::to_string(a_number) + "].linear", m_linear);
	a_shader->SetFloat("pointLights[" + std::to_string(a_number) + "].quadratic", m_quadratic);
}

void PointLight::OnGUI()
{
	if (ImGui::TreeNode("Light Component"))
	{
		ImGui::Unindent();
		Light::OnGUI();

		// Expose colours and values from base and pointlight here
		ImGui::DragFloat("Attenuation", &m_attenuation, 0.1f, 0);
		ImGui::TreePop();
		ImGui::Indent();
	}
}

void PointLight::PrePass(Shader* a_shader, glm::vec2 a_number)
{

}

void PointLight::PostPass(Shader* a_shader)
{
	m_lightTransform.SetPosition(GetOwnerEntity()->GetTransform()->GetPosition());

	a_shader->SetVec3("lightColour", m_diffuseColour, true);
	a_shader->SetMat4("model", *m_lightTransform.GetMatrix(), true);
	m_cubeRepresentation->RenderCube(a_shader);
}
