#include "Component.h"
#include <glm/ext.hpp>
#include "Entity.h"
#include "imgui.h"



Component::Component(Entity* a_pOwnerEntity)
	: m_pOwnerEntity(a_pOwnerEntity),
	m_bActive(true),
	m_eComponentType(TRANSFORM)
{

}

Component::~Component()
{
}

void Component::Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_uIBO)
{
}

void Component::Update(float a_fDeltaTime)
{
}

void Component::OnGUI()
{
	switch (m_eComponentType)
	{
	case TRANSFORM:
	{
		ImGui::TextColored(ImVec4(255, 255, 255, 1), "Transform");
		break;
	}
	case MODEL:
	{
		ImGui::TextColored(ImVec4(255, 255, 255, 1), "Model Component");
		break;
	}
	case CAMERA:
	{
		ImGui::TextColored(ImVec4(255, 255, 255, 1), "Camera");
		break;
	}

	default:
		break;
	}

	ImGui::PushID(m_eComponentType);
	if (!m_bActive)
	{
		if (ImGui::Button("Activate"))
		{
			m_bActive = true;
		}
	}
	else
	{
		if (ImGui::Button("Deactivate"))
		{
			m_bActive = false;
		}
	}
	ImGui::PopID();
	ImGui::NewLine();

}

COMPONENT_TYPE Component::GetComponentType()
{
	return m_eComponentType;

}

void Component::SetComponentType(COMPONENT_TYPE a_type)
{
	m_eComponentType = a_type;
}

Entity* Component::pGetOwnerEntity()
{
	return m_pOwnerEntity;
}


