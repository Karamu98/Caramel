#pragma once

#include <glm/glm.hpp>
#include "Render/Shader.h"
#include "Transform.h"


namespace Caramel
{
	class Camera
	{
	public:
		Camera();

		void Draw(std::shared_ptr<Shader> a_program);
		void Update(float a_deltaTime);
		void SetProjectionMatrix(glm::mat4 a_newMat);
		void SetRatio(unsigned int a_w, unsigned int a_h);
		glm::mat4 GetProjectionMatrix();
		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjectionView();

		inline Caramel::Transform* Transform() { return &m_Transform; }

	private:

		Caramel::Transform m_Transform;
		glm::mat4 m_projMatrix;
		glm::mat4 m_projViewMatrix;

		float m_camSpeed = 2;
	};
}