#ifndef __SCENE_H_
#define __SCENE_H_

#include <vector>
#include "Component.h"
#include <map>
#include "Entity.h"

class Camera;

class Scene
{
public:
	Scene();
	~Scene();
	void Update(float a_delta);
	Entity* Add(Entity* a_toAdd);
	Camera* GetActiveCamera();
	template <typename T> std::vector<T*> FindAllComponentsOfType();
	void Delete();
	void Delete(Entity* a_toDelete);

	std::vector<Entity*> m_sceneEntities;
	Entity* selectedEntity;	

	Camera* m_activeCamera;
};


template<typename T>
std::vector<T*> Scene::FindAllComponentsOfType()
{
	std::vector<T*> compList;

	for (Entity* entity : m_sceneEntities)
	{
		std::vector<T*> entityComps = entity->FindComponentsOfType<T>();

		// Add it
		compList.insert(compList.end(), entityComps.begin(), entityComps.end());
	}
	return compList;
}

#endif // !__SCENE_H_