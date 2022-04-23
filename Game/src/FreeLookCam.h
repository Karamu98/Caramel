#pragma once

#include "Caramel.h"

class FreeLookCam
{
public:
	FreeLookCam();
	FreeLookCam(Caramel::Scene* scene);

	void Update(float deltaTime);

	Caramel::Entity& Entity() { return m_entity; }

private:
	Caramel::Components::Transform* m_transform;
	Caramel::Components::Camera* m_camera;
	Caramel::Entity m_entity;

	double m_mouseLastX;
	double m_mouseLastY;
	float m_cameraSpeed;
};

