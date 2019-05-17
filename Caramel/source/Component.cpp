#include "Component.h"
#include <glm/ext.hpp>
#include "Entity.h"
#include "imgui.h"
#include "Defines.h"



Component::Component(Entity* a_pOwnerEntity)
	: m_pOwnerEntity(a_pOwnerEntity),
	m_bActive(true)
{
	m_modelNumber = a_pOwnerEntity->GetComponentList()->size();
	a_pOwnerEntity->AddComponent(this);
}

Component::~Component()
{
}

void Component::Update(float a_fDeltaTime)
{
	if (!m_bActive)
	{
		return;
	}
}

void Component::OnGUI()
{
	ImGui::PushID(m_pOwnerEntity);
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

Entity* Component::GetOwnerEntity()
{
	return m_pOwnerEntity;
}

bool Component::IsEnabled()
{
	return m_bActive;
}

void Component::SetEnabled(bool a_newState)
{
	m_bActive = a_newState;
}

unsigned int Component::GetModelNumber()
{
	return m_modelNumber;
}

