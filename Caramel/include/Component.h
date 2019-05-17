#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include "imgui.h"

class Entity;

class Component
{
public:

	Component(Entity* a_pOwnerEntity);
	virtual ~Component();

	virtual void Update(float a_fDeltaTime);
	virtual void OnGUI();
	virtual bool OnDelete() = 0;
	virtual Component* Duplicate(Entity* a_owner) = 0;

	Entity* GetOwnerEntity();
	bool IsEnabled();
	void SetEnabled(bool a_newState = true);

	unsigned int GetModelNumber();


private:

	Entity* m_pOwnerEntity;
	unsigned int m_modelNumber;
	bool m_bActive;

};
#endif