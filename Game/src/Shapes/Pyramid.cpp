#include "Pyramid.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const float Pyramid::vertexData[] =
{
	// Positions			// ~Normals				// ~Uv's

	// Back face
	 1.0f, -1.0f, -1.0f,	 0.0f,  0.5f, -1.0f,	0.0f, 0.0f, // Far base right
	-1.0f, -1.0f, -1.0f,	 0.0f,  0.5f, -1.0f,	1.0f, 0.0f, // Far base left
	 0.0f,  1.0f,  0.0f,	 0.0f,  0.5f, -1.0f,	0.5f, 1.0f,  // Pyramid peak
							 
	// Front face			 
	-1.0f, -1.0f,  1.0f,	 0.0f,  0.5f,  1.0f,	0.0f, 0.0f, // Close base left
	 1.0f, -1.0f,  1.0f,	 0.0f,  0.5f,  1.0f,	1.0f, 0.0f, // Close base right
	 0.0f,  1.0f,  0.0f,	 0.0f,  0.5f,  1.0f,	0.5f, 1.0f, // Pyramid peak

	// Left face
	-1.0f, -1.0f, -1.0f,	-1.0f,  0.5f,  0.0f,	1.0f, 0.0f, // Far base left
	-1.0f, -1.0f,  1.0f,	-1.0f,  0.5f,  0.0f,	0.0f, 0.0f, // Close base left
	 0.0f,  1.0f,  0.0f,	-1.0f,  0.5f,  0.0f,	0.5f, 1.0f, // Pyramid peak

	// Right face
	 1.0f, -1.0f,  1.0f,	 1.0f,  0.5f,  0.0f,	1.0f, 0.0f, // Close base right
	 1.0f, -1.0f, -1.0f,	 1.0f,  0.5f,  0.0f,	0.0f, 0.0f, // Far base right
	 0.0f,  1.0f,  0.0f,	 1.0f,  0.5f,  0.0f,	0.5f, 1.0f, // Pyramid peak

	// Bottom face
	-1.0f, -1.0f, -1.0f,	 0.0f,  1.0f,  0.0f,	1.0f, 0.0f, // Far base left
	 1.0f, -1.0f, -1.0f,	 0.0f,  1.0f,  0.0f,	0.0f, 0.0f, // Far base right
	 1.0f, -1.0f,  1.0f,	 0.0f,  1.0f,  0.0f,	1.0f, 0.0f, // Close base right
	 1.0f, -1.0f,  1.0f,	 0.0f,  1.0f,  0.0f,	1.0f, 0.0f, // Close base right
	-1.0f, -1.0f,  1.0f,	 0.0f,  1.0f,  0.0f,	0.0f, 0.0f, // Close base left
	-1.0f, -1.0f, -1.0f,	 0.0f,  1.0f,  0.0f,	1.0f, 0.0f // Far base left
};

Pyramid::Pyramid()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
};

void Pyramid::DrawShape()
{
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 18);
	glBindVertexArray(0);
}