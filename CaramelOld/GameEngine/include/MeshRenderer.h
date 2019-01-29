#ifndef _MESH_RENDERER_H__
#define _MESH_RENDERER_H__

//includes
#include "Component.h"

class Entity;

class MeshRenderer : public Component
{
public:

	MeshRenderer(Entity* a_pOwner);
	~MeshRenderer();

	virtual void Update(float a_fDeltaTime) {};
	virtual void Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_uIBO);
	virtual void OnGUI();

private:
};

#endif