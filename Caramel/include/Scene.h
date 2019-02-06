#ifndef __SCENE_H_
#define __SCENE_H_

#include <vector>
#include "Entity.h"

class Scene
{
public:
	Scene();
	~Scene();
	void Update(float a_delta);
	void Save();
	void Load(const char* a_pathToScene);
	void Add(Entity* a_toAdd);
	void Delete();
	void Delete(Entity* a_toDelete);
	void Duplicate();
	void Duplicate(Entity* a_toCopy);

	std::vector<Entity*> m_sceneEntities;
	Entity* selectedEntity;	
};
#endif // !__SCENE_H_
