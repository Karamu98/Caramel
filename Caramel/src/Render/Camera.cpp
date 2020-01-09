#include "clpch.h"
#include "Camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include <glm/ext.hpp>

#include "Window.h"

namespace Caramel
{
	Camera::Camera()
	{
		glm::vec3 pos = glm::vec3(0, 3, 5);
		m_Transform = glm::inverse(glm::lookAt(pos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
		m_projMatrix = glm::perspective(glm::radians(50.0f), (float)AppWindow::GetWidth() / (float)AppWindow::GetHeight(), 0.1f, 5000.0f);
	}

	void Camera::Draw(std::shared_ptr<Shader> a_program)
	{
		m_projViewMatrix = m_projMatrix * glm::inverse(*m_Transform.GetMatrix());

		a_program->SetMat4("camProjView", m_projViewMatrix);
		a_program->SetVec3("gCamPos", m_Transform.GetPos());
	}

	void Camera::Update(float a_deltaTime)
	{
		GLFWwindow* window = glfwGetCurrentContext();

		// Get the camera's forward, right, up, and location vectors
		glm::vec4 vForward = (*m_Transform.GetMatrix())[2];
		glm::vec4 vRight = (*m_Transform.GetMatrix())[0];
		glm::vec4 vUp = (*m_Transform.GetMatrix())[1];
		glm::vec4 vTranslation = (*m_Transform.GetMatrix())[3];

		// check for camera rotation
		static bool sbMouseButtonDown = false;
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
		{
			static double siPrevMouseX = 0;
			static double siPrevMouseY = 0;

			if (sbMouseButtonDown == false)
			{
				sbMouseButtonDown = true;
				glfwGetCursorPos(window, &siPrevMouseX, &siPrevMouseY);
			}

			double mouseX = 0, mouseY = 0;
			glfwGetCursorPos(window, &mouseX, &mouseY);

			double iDeltaX = mouseX - siPrevMouseX;
			double iDeltaY = mouseY - siPrevMouseY;

			siPrevMouseX = mouseX;
			siPrevMouseY = mouseY;

			glm::mat4 mMat;

			// pitch
			if (iDeltaY != 0)
			{
				mMat = glm::axisAngleMatrix(vRight.xyz(), (float)-iDeltaY / 150.0f);
				vRight = mMat * vRight;
				vUp = mMat * vUp;
				vForward = mMat * vForward;
			}

			// yaw
			if (iDeltaX != 0)
			{
				mMat = glm::axisAngleMatrix(glm::vec3(0, 1, 0), (float)-iDeltaX / 150.0f);
				vRight = mMat * vRight;
				vUp = mMat * vUp;
				vForward = mMat * vForward;
			}

			(*m_Transform.GetMatrix())[0] = vRight;
			(*m_Transform.GetMatrix())[1] = vUp;
			(*m_Transform.GetMatrix())[2] = vForward;
		}
		else
		{
			sbMouseButtonDown = false;
			return;
		}

		float frameSpeed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? a_deltaTime * m_camSpeed * 2 : a_deltaTime * m_camSpeed;

		// Translate camera
		if (glfwGetKey(window, 'W') == GLFW_PRESS)
		{
			vTranslation -= vForward * frameSpeed;
		}
		if (glfwGetKey(window, 'S') == GLFW_PRESS)
		{
			vTranslation += vForward * frameSpeed;
		}
		if (glfwGetKey(window, 'D') == GLFW_PRESS)
		{
			vTranslation += vRight * frameSpeed;
		}
		if (glfwGetKey(window, 'A') == GLFW_PRESS)
		{
			vTranslation -= vRight * frameSpeed;
		}
		if (glfwGetKey(window, 'Q') == GLFW_PRESS)
		{
			vTranslation += vUp * frameSpeed;
		}
		if (glfwGetKey(window, 'E') == GLFW_PRESS)
		{
			vTranslation -= vUp * frameSpeed;
		}

		(*m_Transform.GetMatrix())[3] = vTranslation;
	}

	void Camera::SetProjectionMatrix(glm::mat4 a_newMat)
	{
		m_projMatrix = a_newMat;
	}
	void Camera::SetRatio(unsigned int a_w, unsigned int a_h)
	{
		m_projMatrix = glm::perspective(glm::radians(50.0f), (float)a_w / (float)a_h, 0.1f, 5000.0f);
	}
	glm::mat4 Camera::GetProjectionMatrix()
	{
		return m_projMatrix;
	}
	glm::mat4 Camera::GetViewMatrix()
	{
		return glm::inverse((*m_Transform.GetMatrix()));
	}
	glm::mat4 Camera::GetProjectionView()
	{
		return m_projMatrix * GetViewMatrix();
	}
}