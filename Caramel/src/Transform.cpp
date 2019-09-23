#include "clpch.h"
#include "Transform.h"

#include <glm/ext.hpp>


namespace Caramel
{
	Transform::Transform()
	{
		m_matrix = glm::inverse(glm::lookAt(glm::vec3(0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0)));
	}

	Transform::Transform(glm::mat4x4 a_mat)
	{
		m_matrix = a_mat;
	}

	Transform::Transform(glm::vec3 a_pos)
	{
		m_matrix = glm::inverse(glm::lookAt(a_pos, glm::vec3(0, 0, 1), glm::vec3(0, 1, 0)));
	}

	Transform::Transform(glm::vec3 a_pos, glm::vec3 a_forward)
	{
		m_matrix = glm::inverse(glm::lookAt(a_pos, glm::vec3(0, 0, 1), a_forward));
	}

	void Transform::SetPosition(glm::vec3 a_newPos)
	{
		m_matrix[3] = glm::vec4(a_newPos, 1);
	}

	void Transform::LookAt(glm::vec3 a_pos)
	{
		m_matrix = glm::lookAt(GetPos(), a_pos, glm::vec3(0, 1, 0));
	}

	void Transform::LookAt(Transform& a_pos)
	{
		m_matrix = glm::inverse(glm::lookAt(GetPos(), a_pos.GetPos(), glm::vec3(0, 1, 0)));
	}

	void Transform::Rotate(float a_angle, glm::vec3 a_axis)
	{
		m_matrix = glm::rotate(m_matrix, a_angle, a_axis);
	}

	void Transform::Scale(glm::vec3 a_newScale)
	{
		m_matrix[0] = glm::vec4(1, 0, 0, 0) * a_newScale.x;
		m_matrix[1] = glm::vec4(0, 1, 0, 0) * a_newScale.y;
		m_matrix[2] = glm::vec4(0, 0, 1, 0) * a_newScale.z;
	}
}