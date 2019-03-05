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

	void bind() {
		glBindBuffer(GL_VERTEX_ARRAY, m_vbufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibufferObject);
	}

	void unBind() { glBindBuffer(GL_VERTEX_ARRAY, 0); glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
	
	void FillVBO(void* a_data, unsigned int a_num)
	{
		glBufferData(GL_VERTEX_ARRAY, sizeof(Vertex) * a_num, a_data, 0);
	}

	void subVBOData(void* a_data, unsigned int a_num)
	{
		glBufferSubData(GL_VERTEX_ARRAY, char*(0) + offset, sizeof(Vertex), a_data); // change a prt of the data
	}
	/// Mesh Data
	Vertex* verts;
	int vertCount;

private:
	const char* m_modelPath;

	char* m_textBuffer; // For ImGui

	Material* m_material;

	unsigned int m_vbufferObject;
	unsigned int m_ibufferObject;
	
};

#endif