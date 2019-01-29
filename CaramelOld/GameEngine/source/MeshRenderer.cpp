#include "MeshRenderer.h"
#include "..\include\MeshRenderer.h"

typedef Component PARENT;

MeshRenderer::MeshRenderer(Entity * a_pOwner) : PARENT(a_pOwner)
{
	SetComponentType(MESHRENDERER);
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_uIBO)
{
}

void MeshRenderer::OnGUI()
{
}
