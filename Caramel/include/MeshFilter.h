#ifndef _MESH_FILTER_H__
#define _MESH_FILTER_H__

//includes
#include "Component.h"
#include <string>
#include <glm/glm.hpp>

class Entity;
class Material;
class Shader;
class Mesh;
class Texture;

class MeshFilter : public Component
{
public:

	MeshFilter(Entity* a_pOwner);
	~MeshFilter();

	virtual void Update(float a_fDeltaTime) {};
	virtual void OnGUI();

	void LoadModel();
	void Draw(Shader* a_shader);

	// Helper functions
	void SetupMesh();
	void Bind();
	void Unbind();
	
	//void FillVBO(void* a_data, unsigned int a_num)
	//{
	//	glBufferData(GL_VERTEX_ARRAY, sizeof(Vertex) * a_num, a_data, 0);
	//}

	//void subVBOData(void* a_data, unsigned int a_num)
	//{
	//	glBufferSubData(GL_VERTEX_ARRAY, char*(0) + offset, sizeof(Vertex), a_data); // change a prt of the data
	//}


	/// Mesh Data


private:
	const char* m_modelPath;

	char* m_textBuffer; // For ImGui

	Material* m_material;
	
};

#endif