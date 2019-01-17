#include "MeshFilter.h"

typedef Component PARENT;

MeshFilter::MeshFilter(Entity * a_pOwner) : PARENT(a_pOwner)
{
	SetComponentType(MESHFILTER);
}

MeshFilter::~MeshFilter()
{
}

void MeshFilter::OnGUI()
{
}

void MeshFilter::Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_uIBO)
{
}
