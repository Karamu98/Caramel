#ifndef __GIZMOMESH_H_
#define __GIZMOMESH_H_

#include "Component.h"

typedef enum GizmoMeshType
{
	SQUARE,
	BOX,
	CIRCLE,
	SPHERE,
	TRIANGLE,
	TEAPOT
}GizmoMeshType;

class GizmoMesh : public Component
{
public:
	GizmoMesh(Entity* a_pOwner);
	~GizmoMesh();

	virtual void Update(float a_fDeltaTime);
	virtual void Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_uIBO);
	virtual void OnGUI();

private:

	GizmoMeshType m_type; 
};

#endif // !
