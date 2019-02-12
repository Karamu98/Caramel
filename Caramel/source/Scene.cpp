#include "Scene.h"
#include "Entity.h"

Scene::Scene()
{
	
}

Scene::~Scene()
{
	// Delete all of our entities
	std::vector< Entity* >::iterator xIter;
	for (xIter = m_sceneEntities.begin(); xIter < m_sceneEntities.end(); ++xIter)
	{
		Entity* pCurrentEntity = *xIter;
		if (pCurrentEntity)
		{
			delete pCurrentEntity;
		}
	}
}

void Scene::Update(float a_delta)
{
	// Update all of our entities
	std::vector< Entity* >::iterator xIter;
	for (xIter = m_sceneEntities.begin(); xIter < m_sceneEntities.end(); ++xIter)
	{
		Entity* pCurrentEntity = *xIter;
		if (pCurrentEntity)
		{
			pCurrentEntity->Update(a_delta);
		}
	}
}

void Scene::Add(Entity * a_toAdd)
{
	m_sceneEntities.push_back(a_toAdd);
	selectedEntity = a_toAdd;
}

void Scene::Delete()
{
	ptrdiff_t old = std::find(m_sceneEntities.begin(), m_sceneEntities.end(), selectedEntity) - m_sceneEntities.begin();

	m_sceneEntities.erase(m_sceneEntities.begin() + old);

	if (m_sceneEntities.empty())
	{
		selectedEntity = nullptr;
		return;
	}
	selectedEntity = m_sceneEntities.at(0);
}

void Scene::Delete(Entity * a_toDelete)
{
	ptrdiff_t old = std::find(m_sceneEntities.begin(), m_sceneEntities.end(), a_toDelete) - m_sceneEntities.begin();

	m_sceneEntities.erase(m_sceneEntities.begin() + old);
	selectedEntity = m_sceneEntities.at(0);
}

void Scene::Duplicate()
{
}

void Scene::Duplicate(Entity * a_toCopy)
{
}

Component* Scene::FindComponentOfType(COMPONENT_TYPE a_type)
{
	Component* comp;
	for (int i = 0; i < m_sceneEntities.size; i++)
	{
		comp = m_sceneEntities.at(i)->FindComponentOfType(a_type);

		if (comp != nullptr)
		{
			return comp;
		}
	}
}

std::vector<Component*> Scene::FindComponentsOfType(COMPONENT_TYPE a_type)
{
	std::vector<Component*> list;
	Component* comp;
	for (int i = 0; i < m_sceneEntities.size; i++)
	{
		comp = m_sceneEntities.at(i)->FindComponentOfType(a_type);

		if (comp != nullptr)
		{
			list.push_back(comp);
		}
	}

	return list;
}
