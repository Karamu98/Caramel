#include "Entity.h"
#include "TransformComponent.h"
#include "imgui.h"

#include "Component.h"
#include <iostream>
#include <string>

unsigned int Entity::s_uiEntityIDCount = 0;

std::map<const unsigned int, Entity*> Entity::s_xEntityMap;

typedef std::pair<const unsigned int, Entity*> EntityPair;


Entity::Entity()
{
	//Increment entity ID
	m_uiEntityID = s_uiEntityIDCount++;

	//Add entity to list
	s_xEntityMap.insert(EntityPair(m_uiEntityID, this));

	SetName(std::string("Default ") += std::to_string(GetEntityID()));

	AddComponent(new TransformComponent(this));

	m_rootTransformComponent = static_cast<TransformComponent*>(FindComponentOfType(TRANSFORM));
}

Entity::Entity(Entity * a_original)
{
	//Increment entity ID
	m_uiEntityID = s_uiEntityIDCount++;

	//Add entity to list
	s_xEntityMap.insert(EntityPair(m_uiEntityID, this));

	SetName(std::string(a_original->GetName()->c_str()) += std::to_string(GetEntityID()));

	DuplicateComponents(a_original);
	AddComponent(new TransformComponent(this)); // Temp while duplicate is broken

	m_rootTransformComponent = static_cast<TransformComponent*>(FindComponentOfType(TRANSFORM));

}


Entity::~Entity()
{
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

void Entity::Draw(unsigned int a_uiProgramID, unsigned int a_uiVBO, unsigned int a_uiIBO)
{
	std::vector<Component*>::iterator xIter;
	for (xIter = m_apComponentList.begin(); xIter < m_apComponentList.end(); ++xIter)
	{
		Component* pComponent = *xIter;
		if (pComponent)
		{
			pComponent->Draw(a_uiProgramID, a_uiVBO, a_uiIBO);
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

void Entity::DuplicateComponents(Entity* a_original)
{
	/*std::vector<Component*>::iterator xIter;
	for (xIter = a_original->GetComponentList()->begin(); xIter < a_original->GetComponentList()->end(); ++xIter)
	{
		Component* pOriginalComp = *xIter;
		Component* newComp = new Component(this);

		*newComp = *pOriginalComp;

		AddComponent(newComp);
	}*/
}

Component* Entity::FindComponentOfType(COMPONENT_TYPE eComponentType)
{
	std::vector<Component*>::iterator xIter;
	for (xIter = m_apComponentList.begin(); xIter < m_apComponentList.end(); ++xIter)
	{
		Component* pComponent = *xIter;
		if (pComponent && pComponent->GetComponentType() == eComponentType)
		{
			return pComponent;
		}
	}
	return nullptr;
}

std::vector<Component*>* Entity::GetComponentList()
{
	return &m_apComponentList;
}

TransformComponent * Entity::pGetRootTransformComp()
{
	return m_rootTransformComponent;
}

std::string* Entity::GetName()
{
	return &ssName;
}

void Entity::SetName(std::string a_newName)
{
	ssName = a_newName;
}

std::map<const unsigned int, Entity*> Entity::GetEntityMap()
{
	return s_xEntityMap;
}
