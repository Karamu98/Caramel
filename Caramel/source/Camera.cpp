#include "Camera.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "GLFW/glfw3.h"
#include "Defines.h"
#include "Utilities.h"

typedef Component PARENT;

Camera::Camera(Entity * a_pOwner) : PARENT(a_pOwner)
{
	SetComponentType(CAMERA);

	glm::vec3* pos = &pGetOwnerEntity()->pGetRootTransformComp()->GetCurrentPosition();

	m_cameraMatrix = glm::inverse(glm::lookAt(*pos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0))); // Spawn a camera looking at origin at the entity pos
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, DEFAULT_SCREENWIDTH / (float)DEFAULT_SCREENHEIGHT, 0.1f, 1000.0f); // Create a perspective projection matrix with a 90 degree field-of-view and widescreen aspect ratio
}

Camera::~Camera()
{
}

void Camera::Update(float a_fDeltaTime)
{
	PARENT::Update(a_fDeltaTime);

	glm::mat4* rootTransform = pGetOwnerEntity()->pGetRootTransformComp()->pGetTransformMatrix();

	if (m_bIsPossesed)
	{
		Utility::freeMovement(*rootTransform, a_fDeltaTime, m_cameraSpeed, glm::vec3(0, 1, 0)); // Replace speed and Up with variables from attached transform comp
	}
	m_cameraMatrix = *rootTransform;

	m_cameraMatrix[3] = m_cameraMatrix[3] + m_localPosition;

	// Create a perspective projection matrix with a 90 degree field-of-view and widescreen aspect ratio
	m_projectionMatrix = glm::perspective(glm::pi<float>() * (m_fov / 360), DEFAULT_SCREENWIDTH / (float)DEFAULT_SCREENHEIGHT, 0.1f, 5000.0f);
}

void Camera::OnGUI()
{
}

glm::mat4 Camera::GetCameraMatrix()
{
	return m_cameraMatrix;
}

glm::mat4 Camera::GetProjection()
{
	return glm::perspective(glm::pi<float>() * (m_fov / 360), DEFAULT_SCREENWIDTH / (float)DEFAULT_SCREENHEIGHT, 0.1f, 5000.0f);
}
