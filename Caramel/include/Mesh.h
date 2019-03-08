#ifndef __MESH_H_
#define __MESH_H_

#include <vector>
#include "glm/glm.hpp"

class Texture;
class Shader;

struct Vertex
{
public:
	glm::vec3 position;
	glm::vec3 normals;
	glm::vec2 uvs;
	glm::vec3 tans;
	glm::vec3 biTans;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> a_verts, std::vector<unsigned int> a_indices, std::vector<Texture> a_textures);

	void Draw(Shader* a_shader);

private:

	void Setup();

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	unsigned int m_vBufferObject; // VBO
	unsigned int m_vArrayObject; // VAO
	unsigned int m_iBufferObject; // IBO
};


#endif