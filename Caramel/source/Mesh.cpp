#include "Mesh.h"
#include "gl_core_4_4.h"
#include "Shader.h"
#include <string>
#include "Texture.h"
#include <glm/gtc/type_ptr.hpp>


Mesh::Mesh(std::vector<Vertex> a_verts, std::vector<unsigned int> a_indices, std::vector<Texture> a_textures)
{
	vertices = a_verts;
	indices = a_indices;
	textures = a_textures;

	Setup();
}

void Mesh::Draw(Shader* a_shader)
{
	/// https://learnopengl.com system for drawing models
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = textures[i].m_type;
		if (name == "texture_diffuse")
		{
			number = std::to_string(diffuseNr++);
		}
		else if (name == "texture_specular")
		{
			number = std::to_string(specularNr++);
		}

		a_shader->SetInt((name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].m_textureID);
	}
	glActiveTexture(GL_TEXTURE0);

	// Draw mesh
	glBindVertexArray(m_vArrayObject);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::Unload()
{
	vertices.clear();
	indices.clear();
	for (Texture tex : textures)
	{
		tex.Unload();
	}
	textures.clear();
	glDeleteBuffers(1, &m_vBufferObject);
	glDeleteBuffers(1, &m_iBufferObject);
	glDeleteVertexArrays(1, &m_vArrayObject);
}

void Mesh::Setup()
{
	// Generate the required buffers
	glGenVertexArrays(1, &m_vArrayObject);
	glGenBuffers(1, &m_vBufferObject);
	glGenBuffers(1, &m_iBufferObject);

	glBindVertexArray(m_vArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_vBufferObject);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW); // Copy all of the vertices into our buffer

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW); // Copy all of the indices into our buffer

	// Vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	// Vertex normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)offsetof(Vertex, normals));
	glEnableVertexAttribArray(1);
	// Vertex tans
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)offsetof(Vertex, tans));
	glEnableVertexAttribArray(2);
	// Vertex bitans
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)offsetof(Vertex, biTans));
	glEnableVertexAttribArray(3);
	// Vertex uvs
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)offsetof(Vertex, uvs));
	glEnableVertexAttribArray(4);

	glBindVertexArray(0);

}
