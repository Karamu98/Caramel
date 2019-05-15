#include "Camera.h"
#include "Entity.h"
#include "GLFW/glfw3.h"
#include "Defines.h"
#include "Utilities.h"


typedef Component PARENT;

Camera::Camera(Entity * a_pOwner) : PARENT(a_pOwner),
m_fov(90),
m_cameraSpeed(20),
m_bIsPossesed(true)
{
	m_localPosition = glm::vec4(0, 5, 0, 1); // Set the camera just above root	

	glm::vec3 pos = GetOwnerEntity()->GetTransform()->GetPosition() + glm::vec3(m_localPosition);

	m_cameraMatrix = glm::inverse(glm::lookAt(pos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	m_projectionMatrix = glm::perspective(glm::radians(m_fov * 0.5f), *g_ScreenWidth / (float)*g_ScreenHeight, 0.1f, 5000.0f); // Create a perspective projection matrix with a 90 degree field-of-view and widescreen aspect ratio
}

Camera::~Camera()
{

}

void Camera::Update(float a_fDeltaTime)
{
	PARENT::Update(a_fDeltaTime);

	// If the camera is possessed (User controlled), update the root transform and the local offset
	if (m_bIsPossesed)
	{
		// Grabbing a pointer to the root transform
		glm::mat4* rootTransform = GetOwnerEntity()->GetTransform()->GetMatrix();

		// Use utilities to update the root transform
		Utility::freeMovement(*rootTransform, a_fDeltaTime, m_cameraSpeed, glm::vec3(0, 1, 0)); // Replace speed and Up with variables from attached transform comp
		
		// Update the local offset
		m_cameraMatrix = *rootTransform;

		// For FOV Slider, this needs to be recalculated
		m_projectionMatrix = glm::perspective(glm::radians(m_fov * 0.5f), *g_ScreenWidth / (float)* g_ScreenHeight, 0.1f, 5000.0f); 
	}
}

void Camera::OnGUI()
{
	// List Transfrom Component
	if (ImGui::TreeNode("Camera Component"))
	{
		ImGui::Unindent();
		ImGui::SliderFloat("FOV", &m_fov, 10.0f, 170.0f);
		ImGui::SliderFloat("Camera Speed", &m_cameraSpeed, 0.1f, 200.0f);
		ImGui::TreePop();
		ImGui::Indent();
	}
}

bool Camera::OnDelete()
{
	ImGui::PushID(GetOwnerEntity() + GetModelNumber());
	if (ImGui::Button("Camera"))
	{
		GetOwnerEntity()->DeleteComponent(this);
		ImGui::PopID();
		return true;
	}
	else
	{
		ImGui::PopID();
		return false;
	}
}

glm::mat4 Camera::GetCameraMatrix()
{
	return m_cameraMatrix;
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return m_projectionMatrix;
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::inverse(m_cameraMatrix);
}

glm::mat4 Camera::GetProjectionView()
{
	return m_projectionMatrix * GetViewMatrix();
}
