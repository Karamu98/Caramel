#include "Entity.h"
#include "Transform.h"
#include "imgui.h"
#include "Gizmos.h"
#include "Component.h"
#include <iostream>
#include <string>
#include "Log.h"
#include "Camera.h"
#include "Scene.h"

unsigned int Entity::s_uiEntityIDCount = 0;

Entity::Entity(Scene* a_scene)
{
	a_scene->Add(this);
	//Increment entity ID
	m_uiEntityID = s_uiEntityIDCount++;

	SetName(std::string("Default ") += std::to_string(GetEntityID()));
}

Entity::~Entity()
{
	s_uiEntityIDCount--;
	for (Component* comp : m_apComponentList)
	{
		if (comp != nullptr)
		{
			delete comp;
		}
	}

}

void Entity::Update(float a_fDeltaTime)
{
	std::vector<Component*>::iterator xIter;
	for(xIter = m_apComponentList.begin(); xIter < m_apComponentList.end(); ++xIter)
	{
		Component* pComponent = *xIter;
		if (pComponent)
		{
			pComponent->Update(a_fDeltaTime);
		}
	}
}

void Entity::OnGUI()
{
	Gizmos::addTransform(*m_Transform.GetMatrix(), 1);

	auto positionRow = &((*m_Transform.GetMatrix())[3]);

	// List Transfrom Component
	ImGui::TextColored(ImVec4(255, 255, 255, 1), "Transform Component");
	ImGui::Text("Name: ");
	ImGui::SameLine(50);
	ImGui::InputText("", (char*)GetName()->c_str(), 10);
	ImGui::Text("Position: ");
	ImGui::InputFloat(": X", &((*positionRow)[0]));
	ImGui::InputFloat(": Y", &((*positionRow)[1]));
	ImGui::InputFloat(": Z", &((*positionRow)[2]));
	//ImGui::Text("Position X: %i Y: %i Z: %i", (int)pGetOwnerEntity()->pGetRootTransformComp()->GetCurrentPosition().x, (int)pGetOwnerEntity()->pGetRootTransformComp()->GetCurrentPosition().y, (int)pGetOwnerEntity()->pGetRootTransformComp()->GetCurrentPosition().z);
	if (ImGui::Button("Reset"))
	{
		m_Transform.Reset();
	}
	ImGui::NewLine();


	std::vector<Component*>::iterator xIter;
	for (xIter = m_apComponentList.begin(); xIter < m_apComponentList.end(); ++xIter)
	{
		Component* pComponent = *xIter;
		if (pComponent)
		{
			pComponent->OnGUI();
		}
	}

}

void Entity::AddComponent(Component * a_pComponent)
{
	m_apComponentList.push_back(a_pComponent);
}

void Entity::DeleteComponent(Component* a_pComponent)
{
	ptrdiff_t old = std::find(m_apComponentList.begin(), m_apComponentList.end(), a_pComponent) - m_apComponentList.begin();

	m_apComponentList.erase(m_apComponentList.begin() + old);
	delete a_pComponent;
}

std::vector<Component*>* Entity::GetComponentList()
{
	return &m_apComponentList;
}

Transform* Entity::GetTransform()
{
	return &m_Transform;
}

std::string* Entity::GetName()
{
	return &ssName;
}

void Entity::SetName(std::string a_newName)
{
	ssName = a_newName;
}