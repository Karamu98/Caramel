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
	~Entity();

	virtual void Update(float a_fDeltaTime);
	virtual void OnGUI();

	void AddComponent(Component* a_pComponent);

	template <typename T> T* GetComponentOfType(int a_element = 0) const;
	std::vector<Component*>* GetComponentList();

	TransformComponent* GetRootTransform();

	std::string* GetName();
	void SetName(std::string a_newName);

	inline unsigned int GetEntityID() { return m_uiEntityID; }

private:
	unsigned int m_uiEntityID;

	std::string ssName;
	TransformComponent* m_rootTransform;
	
	std::vector<Component*>m_apComponentList;

	static unsigned int s_uiEntityIDCount;
};


#endif // _ENTITY_H

template<typename T>
T * Entity::GetComponentOfType(int a_element) const
{
	Component* pComponent;
	std::vector<T*> allComps; // Stores a list of identicle typed' components

	// Iterates over a list of components
	for (int i = 0; i < m_apComponentList.size(); i++)
	{
		pComponent = m_apComponentList[i];

		T* casted = dynamic_cast<T*>(pComponent); // Cast it to our type

		if (casted != nullptr) // If it's valid
		{
			if (a_element == 0) // and the caller wants the first element
			{
				return casted; // return a Typed pointer to it
			}

			allComps.push_back(casted); // Add it to the list
		}
	}

	if (allComps.size() > a_element) // If the element is in range
	{
		return allComps[a_element]; // Return a typed pointer to it
	}
	else
	{
		return nullptr;
	}
}
