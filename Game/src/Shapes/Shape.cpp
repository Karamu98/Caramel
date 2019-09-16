#include "Shape.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>


Shape::Shape() : m_vbo(-1), m_vao(-1), m_objMatLoc(-1), m_Transform()
{
}

Shape::~Shape()
{
	unsigned int buffers[2] = { m_vbo, m_vao }; 
	glDeleteBuffers(2, buffers);
}

void Shape::Draw(std::shared_ptr<Caramel::Shader> a_program)
{
	a_program->SetMat4("objMatrix", m_Transform);
	DrawShape();
}

Caramel::Transform* Shape::Transform()
{
	return &m_Transform;
}
