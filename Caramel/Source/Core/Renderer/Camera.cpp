#include "clpch.h"
#include "Camera.h"
#include "Core/Application.h"
#include "Core/RenderAPI/Shader.h"

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
			m_viewMatrix = GetRotationMatrix(calculate);
			m_viewMatrix = glm::translate(m_viewMatrix, -Position);
		}
		return m_viewMatrix;
	}

	glm::mat4& Camera::GetRotationMatrix(bool calculate)
	{
		if (calculate)
		{
			m_rotationMatrix = glm::mat4(1);
			m_rotationMatrix = glm::rotate(m_rotationMatrix, glm::radians(Rotation.x), glm::vec3(1, 0, 0));
			m_rotationMatrix = glm::rotate(m_rotationMatrix, glm::radians(Rotation.y), glm::vec3(0, 1, 0));
			m_rotationMatrix = glm::rotate(m_rotationMatrix, glm::radians(Rotation.z), glm::vec3(0, 0, 1));
		}

		return m_rotationMatrix;
	}

	glm::vec3 Camera::GetForward()
	{
		glm::vec3 forward;
		forward.x = cos(glm::radians(Rotation.y)) * cos(glm::radians(Rotation.x));
		forward.y = sin(glm::radians(Rotation.x));
		forward.z = sin(glm::radians(Rotation.y)) * cos(glm::radians(Rotation.x));
		return glm::normalize(forward);
	}

	void Camera::Bind(std::shared_ptr<Shader> target)
	{
		if (!target)
		{
			return;
		}
		target->SetValue(UNIFORM_NAME_VIEW_POSITION, ShaderDataType::Float3, glm::value_ptr(Position));
		target->SetValue(UNIFORM_NAME_CAMERA_VIEW, ShaderDataType::Mat4, glm::value_ptr(GetView()));
		target->SetValue(UNIFORM_NAME_CAMERA_PROJECTION, ShaderDataType::Mat4, glm::value_ptr(GetProjection()));
	}
}