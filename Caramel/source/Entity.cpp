#include "Entity.h"
#include "TransformComponent.h"
#include "imgui.h"

#include "Component.h"
#include <iostream>
#include <string>
#include "Log.h"
#include "Camera.h"

unsigned int Entity::s_uiEntityIDCount = 0;

Entity::Entity()
{
	//Increment entity ID
	m_uiEntityID = s_uiEntityIDCount++;

	SetName(std::string("Default ") += std::to_string(GetEntityID()));

	AddComponent(new TransformComponent(this));

	m_rootTransform = GetComponentOfType<TransformComponent>(0);
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

std::vector<Component*>* Entity::GetComponentList()
{
	return &m_apComponentList;
}

TransformComponent * Entity::GetRootTransform()
{
	return m_rootTransform;
}

std::string* Entity::GetName()
{
	return &ssName;
}

void Entity::SetName(std::string a_newName)
{
	ssName = a_newName;
}