// ***********************************************************************
// Assembly         : 
#ifndef _ENTITY_H
#define _ENTITY_H

// Includes
#include <map>
#include <vector>

class Component;
class TransformComponent;
enum COMPONENT_TYPE;

class Entity
{
public:

	Entity();
	Entity(Entity* a_original);
	~Entity();

	virtual void Update(float a_fDeltaTime);
	virtual void OnGUI();

	void AddComponent(Component* a_pComponent);
	void DuplicateComponents(Entity* a_original);

	Component* FindComponentOfType(COMPONENT_TYPE eComponentType);
	std::vector<Component*>* GetComponentList();

	TransformComponent* pGetRootTransformComp();

	std::string* GetName();
	void SetName(std::string a_newName);

	inline unsigned int GetEntityID() { return m_uiEntityID; }

	static std::map<const unsigned int, Entity*> GetEntityMap();

private:
	unsigned int m_uiEntityID;

	std::string ssName;
	TransformComponent* m_rootTransformComponent;
	
	std::vector<Component*>m_apComponentList;

	static unsigned int s_uiEntityIDCount;
	static std::map<const unsigned int, Entity*>s_xEntityMap;
};

#endif // _ENTITY_H