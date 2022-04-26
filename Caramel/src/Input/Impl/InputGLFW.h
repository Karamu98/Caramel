#pragma once

#include "Input/Input.h"

#include "Core/Window.h"

namespace Caramel
{
	class InputGLFW : public Input
	{
	public:
		InputGLFW(std::shared_ptr<Window> inputWindow);
	protected:
		bool GetKeyDownImpl(KeyCode keyCode) override;
		bool GetKeyUpImpl(KeyCode keyCode) override;
		bool GetKeyImpl(KeyCode keyCode) override;
		bool GetMouseButtonImpl(MouseButton button) override;
		bool GetMouseButtonDownImpl(MouseButton button) override;
		bool GetMouseButtonUpImpl(MouseButton button) override;
		std::pair<float, float> GetMousePositionImpl() override;

	private:
		std::shared_ptr<Window> m_inputWindow;
	};
}