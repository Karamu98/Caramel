#pragma once

#include <Caramel.h>

class FreeCamera
{
public:
	FreeCamera();
	Caramel::Camera Camera;

	void ImGui();
	void OnUpdate(Caramel::Timestep ts);

private:
	float m_moveSpeed = 10.0f;
	glm::vec2 m_lookSpeed = { 5.0f, 5.0f };

	glm::vec2 m_lastMousePos;
};

