#pragma once

#include "Core/Input.h"

namespace Caramel
{
	class GLFW_Input : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(KeyCode code) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual glm::vec2 GetMousePositionImpl() override;
		virtual void SetMousePositionImpl(glm::vec2 pos) override;
	};
}


