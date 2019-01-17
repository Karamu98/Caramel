#ifndef _COMPONENT_H_
#define _COMPONENT_H_


class Entity;

enum COMPONENT_TYPE
{
	TRANSFORM,
	MODEL,
	CAMERA,
	MESHFILTER,
	MESHRENDERER
};

class Component
{
public:

	Component(Entity* a_pOwnerEntity);
	~Component();

	virtual void Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_uIBO);
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