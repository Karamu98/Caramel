#pragma once

#include "Caramel/Renderer/Camera.h"

namespace Caramel
{
	class EditorCamera
	{
	public:
		EditorCamera();
		Camera Camera;

		void ImGui();
		void OnUpdate(Timestep ts);

	private:
		float m_moveSpeed = 10.0f;
		glm::vec2 m_lookSpeed = { 5.0f, 5.0f };
		glm::vec2 m_lastMousePos;
	};
}


