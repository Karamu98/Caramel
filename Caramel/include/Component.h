#ifndef _COMPONENT_H_
#define _COMPONENT_H_


class Entity;

enum COMPONENT_TYPE
{
	TRANSFORM,
	MODEL,
	CAMERA,
	MESHFILTER,
	MESHRENDERER,
	LIGHT
};

class Component
{
public:

	Component(Entity* a_pOwnerEntity);
	virtual ~Component();

	virtual void Update(float a_fDeltaTime);
	virtual void OnGUI();

	COMPONENT_TYPE GetComponentType();
	void SetComponentType(COMPONENT_TYPE a_type);
	Entity* pGetOwnerEntity();


private:

	Entity* m_pOwnerEntity;
	COMPONENT_TYPE m_eComponentType;
	bool m_bActive;

};
#endif