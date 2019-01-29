#ifndef _MESH_FILTER_H__
#define _MESH_FILTER_H__

//includes
#include "Component.h"
#include <string>

class Entity;

class MeshFilter : public Component
{
public:

	MeshFilter(Entity* a_pOwner);
	~MeshFilter();

	virtual void Update(float a_fDeltaTime) {};
	virtual void Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_uIBO);
	virtual void OnGUI();

	void LoadModel(std::string a_path);

private:
	const char* m_modelPath;

	char* m_textBuffer;
};

#endif