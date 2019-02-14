#ifndef _CAMERA_COMPONENT_H_
#define _CAMERA_COMPONENT_H_

//includes
#include "Component.h"
#include "glm/ext.hpp"

class Entity;

class Camera : public Component
{
public:

	Camera(Entity* a_pOwner);
	~Camera();

	virtual void Update(float a_fDeltaTime);
	virtual void OnGUI();

	glm::mat4 GetCameraMatrix();
	glm::mat4 GetProjection(); // Returns a new projection matrix


private:

	bool		m_bIsPossesed; // Is this camera receiving input
	float		m_cameraSpeed; // This is the speed that the camera can move on input, overriden by player speed
	float		m_fov; // This is the angle in which the camera can see
	glm::vec4	m_localPosition; // This is the local position of the camera (From ROOT)
	glm::mat4	m_cameraMatrix; // World space position (ROOT + Offset)
	glm::mat4	m_projectionMatrix;
};

#endif