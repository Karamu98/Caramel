#include "clpch.h"
#include "Camera.h"
#include "Core/Application.h"

namespace Caramel
{
	Camera::Camera(Window* window)
	{
		m_window = window;
		if (window == nullptr)
		{
			m_window = Application::Get()->GetWindow();
		}
	}

	glm::mat4& Camera::GetProjection(bool calculate)
	{
		if (calculate)
		{
			m_projectionMatrix = glm::perspective(glm::radians(FOV), (float)m_window->GetWidth() / (float)m_window->GetHeight(), NearPlane, FarPlane);
		}

		return m_projectionMatrix;
	}

	glm::mat4& Camera::GetView(bool calculate)
	{
		if (calculate)
		{
			m_viewMatrix = glm::mat4(1.0f);
			m_viewMatrix = glm::rotate(m_viewMatrix, glm::radians(Rotation.x), glm::vec3(1, 0, 0));
			m_viewMatrix = glm::rotate(m_viewMatrix, glm::radians(Rotation.y), glm::vec3(0, 1, 0));
			m_viewMatrix = glm::rotate(m_viewMatrix, glm::radians(Rotation.z), glm::vec3(0, 0, 1));
			m_viewMatrix = glm::translate(m_viewMatrix, -Position);
		}
		return m_viewMatrix;
	}
}