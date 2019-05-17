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
	virtual bool OnDelete();
	virtual Component* Duplicate(Entity* a_owner);

	glm::mat4 GetCameraMatrix();
	glm::mat4 GetProjectionMatrix(); // Returns a new projection matrix
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionView();


private:

	bool		m_bIsPossesed; // Is this camera receiving input
	float		m_cameraSpeed; // This is the speed that the camera can move on input
	float		m_fov; // This is the angle in which the camera can see, this is halved and applied to Projection
	glm::vec4	m_localPosition; // This is the local position of the camera (From ROOT)
	glm::mat4	m_cameraMatrix; // World space position (ROOT + Offset)
	glm::mat4	m_projectionMatrix; // The frustrum of view for the camera
};

#endif