#include "Shape.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>


Shape::Shape() : m_vbo(-1), m_vao(-1), m_objMatLoc(-1)
{
	m_objMatrix = glm::inverse(glm::lookAt(glm::vec3(0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0)));
}

Shape::~Shape()
{
	unsigned int buffers[2] = { m_vbo, m_vao }; 
	glDeleteBuffers(2, buffers);
}

void Shape::Draw(std::shared_ptr<Caramel::Shader> a_program)
{
	a_program->SetMat4("objMatrix", m_objMatrix);
	DrawShape();
}

void Shape::SetPosition(glm::vec3 a_newPos)
{
	m_objMatrix[3] = glm::vec4(a_newPos, 1);
}

void Shape::Rotate(float a_angle, glm::vec3 a_axis)
{
	m_objMatrix = glm::rotate(m_objMatrix, a_angle, a_axis);
}

void Shape::Scale(glm::vec3 a_newScale)
{
	m_objMatrix[0] = glm::vec4(1, 0, 0, 0) * a_newScale.x;
	m_objMatrix[1] = glm::vec4(0, 1, 0, 0) * a_newScale.y;
	m_objMatrix[2] = glm::vec4(0, 0, 1, 0) * a_newScale.z;
}
