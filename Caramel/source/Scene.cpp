#include "Scene.h"
#include "Log.h"
#include "Entity.h"
#include "Camera.h"
#include "Defines.h"


Scene::Scene()
{

}

Scene::~Scene()
{
	Entity* pCurEntity;
	// Delete all of our entities
	for (int i = 0; i < m_sceneEntities.size(); i++)
	{
		pCurEntity = m_sceneEntities[i];
		if (pCurEntity)
		{
			delete pCurEntity;
		}
	}
}

void Scene::Update(float a_delta)
{
	Entity* pCurEntity;
	// Update all of our entities
	for (int i = 0; i < m_sceneEntities.size(); i++)
	{
		pCurEntity = m_sceneEntities[i];
		if (pCurEntity)
		{
			pCurEntity->Update(a_delta);
		}
	}
}

Entity* Scene::Add(Entity * a_toAdd)
{
	m_sceneEntities.push_back(a_toAdd);
	selectedEntity = a_toAdd;
	return a_toAdd;
}

Camera* Scene::GetActiveCamera()
{
	// If our camera is not valid, find another
	if (m_activeCamera == nullptr)
	{
		// Get all cameras in the scene
		std::vector<Camera*> sceneCameras = FindAllComponentsOfType<Camera>();

		// Test for first enabled camera
		for (Camera* cam : sceneCameras)
		{
			if (cam->IsEnabled())
			{
				m_activeCamera = cam;
				break;
			}
		}
	}
	return m_activeCamera;
}

void Scene::Delete()
{
	ptrdiff_t old = std::find(m_sceneEntities.begin(), m_sceneEntities.end(), selectedEntity) - m_sceneEntities.begin();

	Camera* camComp = m_sceneEntities.at(old)->GetComponentOfType<Camera>();

	if (camComp != nullptr)
	{
		if (camComp == m_activeCamera)
		{
			m_activeCamera = nullptr;
		}
	}

	delete m_sceneEntities.at(old);
	m_sceneEntities.erase(m_sceneEntities.begin() + old);

	if (m_sceneEntities.empty())
	{
		selectedEntity = nullptr;
		return;
	}
	selectedEntity = m_sceneEntities[0];
	
}

void Scene::Delete(Entity * a_toDelete)
{
	ptrdiff_t old = std::find(m_sceneEntities.begin(), m_sceneEntities.end(), a_toDelete) - m_sceneEntities.begin();

	m_sceneEntities.erase(m_sceneEntities.begin() + old);
	selectedEntity = m_sceneEntities[0];
}

void Scene::Save(std::ostream* a_outFile)
{
	FileHeader header; 
	header.flag = Flags::ENTITYLIST_START;
	header.size = m_sceneEntities.size();
	SaveToFile(a_outFile, header);

	for (Entity* entity : m_sceneEntities)
	{
		entity->Save(a_outFile);
	}
}

void Scene::Load(std::ifstream* a_inFile)
{
	FileHeader header;
	LoadFromFile(a_inFile, &header);

	if (header.flag != Flags::ENTITYLIST_START)
	{
		CL_CORE_ERROR("Save file mismatch, expected 'ENTITYLIST_START'");
		return;
	}

	m_sceneEntities.reserve(header.size);

	for (int i = 0; i < header.size; ++i)
	{
		Entity* newEntity = new Entity(this);
		newEntity->Load(a_inFile);
	}

	selectedEntity = m_sceneEntities.back();

}
