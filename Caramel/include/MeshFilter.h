#ifndef _MESH_FILTER_H__
#define _MESH_FILTER_H__

//includes
#include "Component.h"
#include <string>
#include <vector>
#include <glm/glm.hpp>

class Entity;
class Material;

class Vertex
{
public:
	glm::vec3 position;
	glm::vec3 normals;
	glm::vec3 tans;
	glm::vec3 biTans;
	glm::vec2 uvs;
};

class MeshFilter : public Component
{
public:

	MeshFilter(Entity* a_pOwner);
	~MeshFilter();

	virtual void Update(float a_fDeltaTime) {};
	virtual void OnGUI();

	void LoadModel();

private:
	const char* m_modelPath;

	char* m_textBuffer; // For ImGui

	Material* m_material;

	/// Mesh Data
	Vertex* verts;
	
};

#endif