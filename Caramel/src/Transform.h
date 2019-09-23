#pragma once

#include <glm/glm.hpp>

namespace Caramel
{
	class Transform
	{
	public:
		Transform();
		Transform(glm::mat4x4 a_mat);
		Transform(glm::vec3 a_pos);
		Transform(glm::vec3 a_pos, glm::vec3 a_forward);

		void SetPosition(glm::vec3 a_newPos);
		void LookAt(glm::vec3 a_pos);
		void LookAt(Transform& a_pos);
		void Rotate(float a_angle, glm::vec3 a_axis);
		void Scale(glm::vec3 a_newScale);
		inline glm::vec3 GetPos() { return m_matrix[3]; }
		inline glm::mat4x4* GetMatrix() { return &m_matrix; }

	private:
		glm::mat4x4 m_matrix;
	};
}