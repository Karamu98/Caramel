#ifndef __MESH_H_
#define __MESH_H_

#include <vector>
#include "glm/glm.hpp"

class Texture;
class Shader;

struct Vertex
{
public:
	glm::vec4 position;
	glm::vec4 normals;
	glm::vec4 tans;
	glm::vec4 biTans;
	glm::vec2 uvs;
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