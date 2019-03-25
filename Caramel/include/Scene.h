#ifndef __SCENE_H_
#define __SCENE_H_

#include <vector>
#include "Component.h"
#include <map>

class Scene
{
public:
	Scene();
	~Scene();
	void Update(float a_delta);
	Entity* Add(Entity* a_toAdd);
	void Delete();
	void Delete(Entity* a_toDelete);

	std::vector<Entity*> m_sceneEntities;
	Entity* selectedEntity;	
};
#endif // !__SCENE_H_
