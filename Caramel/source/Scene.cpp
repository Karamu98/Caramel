#include "Scene.h"

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
