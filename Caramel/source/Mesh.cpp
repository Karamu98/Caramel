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
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		a_shader->SetFloat(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].m_textureID);
	}
	glActiveTexture(GL_TEXTURE0);


	glm::mat4 m4ModelMat = glm::mat4();
	//a_shader->SetMat4("Model", ) :::CONTINUE::: You need a way to manipulate all the meshes in a model when it moves, you also need to get world space for any of these meshes
	a_shader->SetMat4("NormalMatrix", glm::transpose(glm::inverse(m4ModelMat)));

	// Draw mesh
	glBindVertexArray(m_vArrayObject);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::Setup()
{
	// Generate the required buffers
	glGenVertexArrays(1, &m_vArrayObject);
	glGenBuffers(1, &m_vBufferObject);
	glGenBuffers(1, &m_iBufferObject);

	glBindVertexArray(m_vArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_vBufferObject);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW); // Copy all of the vertices into our buffer

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW); // Copy all of the indices into our buffer

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// Vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normals));
	// Vertex uvs
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uvs));
	// Vertex tans
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tans));
	// Vertex bitans
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, biTans));

	glBindVertexArray(0);

}
