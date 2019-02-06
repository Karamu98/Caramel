#ifndef _MESH_FILTER_H__
#define _MESH_FILTER_H__

//includes
#include "Component.h"
#include <string>
#include <vector>
#include <glm/glm.hpp>

class Entity;

class Vertex
{
public:
	glm::vec3 position;
	glm::vec3 normals;
	glm::vec3 tans;
	glm::vec3 biTans;
	glm::vec3 uvs;
};

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

	char* m_textBuffer; // For ImGui

	/// Mesh Data
	Vertex* verts;
	
};

#endif