#include "Cube.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const float Cube::vertexData[] = 
{
	// Positions			// Normals				// UV's

	// Back face
	 1.0f, -1.0f, -1.0f,	0.0f,  0.0f, -1.0f,		0.0f,  0.0f,
	-1.0f, -1.0f, -1.0f,	0.0f,  0.0f, -1.0f,		1.0f,  0.0f,
	-1.0f,  1.0f, -1.0f,	0.0f,  0.0f, -1.0f,		1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,	0.0f,  0.0f, -1.0f,		1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,	0.0f,  0.0f, -1.0f,		0.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,	0.0f,  0.0f, -1.0f,		0.0f,  0.0f,
										 
	// Front face						 
	-1.0f, -1.0f,  1.0f,	0.0f,  0.0f,  1.0f,		0.0f,  0.0f,
	 1.0f, -1.0f,  1.0f,	0.0f,  0.0f,  1.0f,		1.0f,  0.0f,
	 1.0f,  1.0f,  1.0f,	0.0f,  0.0f,  1.0f,		1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,	0.0f,  0.0f,  1.0f,		1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,	0.0f,  0.0f,  1.0f,		0.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,	0.0f,  0.0f,  1.0f,		0.0f,  0.0f,
										 
	// Left face						 
	-1.0f,  1.0f,  1.0f,	-1.0f,  0.0f,  0.0f,	1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,	-1.0f,  0.0f,  0.0f,	0.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,	-1.0f,  0.0f,  0.0f,	0.0f,  0.0f,
	-1.0f, -1.0f, -1.0f,	-1.0f,  0.0f,  0.0f,	0.0f,  0.0f,
	-1.0f, -1.0f,  1.0f,	-1.0f,  0.0f,  0.0f,	1.0f,  0.0f,
	-1.0f,  1.0f,  1.0f,	-1.0f,  0.0f,  0.0f,	1.0f,  1.0f,
										 
	// Right face		 
	 1.0f, -1.0f,  1.0f,	1.0f,  0.0f,  0.0f,		0.0f,  0.0f,
	 1.0f, -1.0f, -1.0f,	1.0f,  0.0f,  0.0f,		1.0f,  0.0f,
	 1.0f,  1.0f, -1.0f,	1.0f,  0.0f,  0.0f,		1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,	1.0f,  0.0f,  0.0f,		1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,	1.0f,  0.0f,  0.0f,		0.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,	1.0f,  0.0f,  0.0f,		0.0f,  0.0f,
	
	// Bottom face
	-1.0f, -1.0f, -1.0f,	0.0f, -1.0f,  0.0f,		0.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,	0.0f, -1.0f,  0.0f,		1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,	0.0f, -1.0f,  0.0f,		1.0f,  0.0f,
	 1.0f, -1.0f,  1.0f,	0.0f, -1.0f,  0.0f,		1.0f,  0.0f,
	-1.0f, -1.0f,  1.0f,	0.0f, -1.0f,  0.0f,		0.0f,  0.0f,
	-1.0f, -1.0f, -1.0f,	0.0f, -1.0f,  0.0f,		0.0f,  1.0f,
	
	// Top face
	-1.0f,  1.0f,  1.0f,	0.0f,  1.0f,  0.0f,		0.0f,  0.0f, 
	 1.0f,  1.0f,  1.0f,	0.0f,  1.0f,  0.0f,		1.0f,  0.0f, 
	 1.0f,  1.0f, -1.0f,	0.0f,  1.0f,  0.0f,		1.0f,  1.0f, 
	 1.0f,  1.0f, -1.0f,	0.0f,  1.0f,  0.0f,		1.0f,  1.0f, 
	-1.0f,  1.0f, -1.0f,	0.0f,  1.0f,  0.0f,		0.0f,  1.0f, 
	-1.0f,  1.0f,  1.0f,	0.0f,  1.0f,  0.0f,		0.0f,  0.0f	 
};

Cube::Cube()
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
}

void Cube::DrawShape()
{
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}