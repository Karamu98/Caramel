#pragma once

#include <glm/glm.hpp>

namespace Caramel
{
	class Camera
	{
	public:
		Camera();

		void Draw(unsigned int a_program);
		void Update(float a_deltaTime);
		void SetProjectionMatrix(glm::mat4 a_newMat);
		glm::mat4 GetProjectionMatrix();
		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjectionView();
		inline glm::vec3 GetPos() { return glm::vec3(m_camMatrix[3]); }

	private:

		glm::mat4 m_projMatrix;
		glm::mat4 m_camMatrix;
		glm::mat4 m_projViewMatrix;

		float m_camSpeed = 2;

	};
}