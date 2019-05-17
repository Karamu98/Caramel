#include "PointLight.h"
#include "Shader.h"
#include "Entity.h"
#include "Transform.h"
#include "Cube.h"
#include "Defines.h"

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

Component* PointLight::Duplicate(Entity* a_owner)
{
	PointLight* newCopy = new PointLight(a_owner);
	
	newCopy->m_diffuseColour = this->m_diffuseColour;
	newCopy->m_specularColour = this->m_specularColour;

	newCopy->m_attenuation = this->m_attenuation;
	newCopy->m_linear = this->m_linear;
	newCopy->m_quadratic = this->m_quadratic;
	newCopy->m_atlasIndex = this->m_atlasIndex;
	newCopy->m_lightTransform = this->m_lightTransform;


	newCopy->m_cubeRepresentation = new Cube();
	return newCopy;
}

void PointLight::Save(std::ostream* a_outStream)
{
	FileHeader pntHead;
	pntHead.flag = Flags::PNTLIGHT_START;
	pntHead.size = (sizeof(glm::vec3) * 2) + (sizeof(float) * 3) + sizeof(m_atlasIndex);
	SaveToFile(a_outStream, pntHead);

	SaveToFile(a_outStream, m_diffuseColour);
	SaveToFile(a_outStream, m_specularColour);
	SaveToFile(a_outStream, m_attenuation);
	SaveToFile(a_outStream, m_linear);
	SaveToFile(a_outStream, m_quadratic);
	SaveToFile(a_outStream, m_atlasIndex);
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
