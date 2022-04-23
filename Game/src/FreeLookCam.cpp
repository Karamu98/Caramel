#include "FreeLookCam.h"

#include "Scene/Components.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

FreeLookCam::FreeLookCam(Caramel::Scene* scene) : m_cameraSpeed(2.0f)
{
	m_entity = scene->CreateEntity("FreeLookCam");
	m_transform = &m_entity.AddComponent<Caramel::Components::Transform>();
	m_camera = &m_entity.AddComponent<Caramel::Components::Camera>(m_transform);
}

FreeLookCam::FreeLookCam()
{

}

void FreeLookCam::Update(float deltaTime)
{
	// Get the camera's forward, right, up, and location vectors
	glm::vec4 vForward = (*m_transform->GetMatrix())[2];
	glm::vec4 vRight = (*m_transform->GetMatrix())[0];
	glm::vec4 vUp = (*m_transform->GetMatrix())[1];
	glm::vec4 vTranslation = (*m_transform->GetMatrix())[3];

	if (Caramel::Input::GetMouseButtonDown(1))
	{
		auto pos = Caramel::Cursor::GetPosition();
		m_mouseLastX = pos.x;
		m_mouseLastY = pos.y;
	}

	if (Caramel::Input::GetMouseButton(1))
	{
		double curMouseX = 0, curMouseY = 0;
		auto pos = Caramel::Cursor::GetPosition();
		curMouseX = pos.x;
		curMouseY = pos.y;

		double mouseDeltaX = curMouseX - m_mouseLastX;
		double mouseDeltaY = curMouseY - m_mouseLastY;

		m_mouseLastX = curMouseX;
		m_mouseLastY = curMouseY;

		glm::mat4 mMat;

		// pitch
		if (mouseDeltaY != 0)
		{
			mMat = glm::axisAngleMatrix(vRight.xyz(), (float)-mouseDeltaY / 150.0f);
			vRight = mMat * vRight;
			vUp = mMat * vUp;
			vForward = mMat * vForward;
		}

		// yaw
		if (mouseDeltaX != 0)
		{
			mMat = glm::axisAngleMatrix(glm::vec3(0, 1, 0), (float)-mouseDeltaX / 150.0f);
			vRight = mMat * vRight;
			vUp = mMat * vUp;
			vForward = mMat * vForward;
		}

		(*m_transform->GetMatrix())[0] = vRight;
		(*m_transform->GetMatrix())[1] = vUp;
		(*m_transform->GetMatrix())[2] = vForward;
	}

	float frameSpeed = Caramel::Input::GetKey(GLFW_KEY_LEFT_SHIFT) ? deltaTime * m_cameraSpeed * 2 : deltaTime * m_cameraSpeed;

	// Translate camera
	if (Caramel::Input::GetKey('W'))
	{
		vTranslation -= vForward * frameSpeed;
	}
	if (Caramel::Input::GetKey('S'))
	{
		vTranslation += vForward * frameSpeed;
	}
	if (Caramel::Input::GetKey('D'))
	{
		vTranslation += vRight * frameSpeed;
	}
	if (Caramel::Input::GetKey('A'))
	{
		vTranslation -= vRight * frameSpeed;
	}
	if (Caramel::Input::GetKey('Q'))
	{
		vTranslation += vUp * frameSpeed;
	}
	if (Caramel::Input::GetKey('E'))
	{
		vTranslation -= vUp * frameSpeed;
	}

	(*m_transform->GetMatrix())[3] = vTranslation;
}
