#pragma once

#include "Caramel/Core/Input.h"

namespace Caramel 
{
	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int a_keycode);

		virtual bool IsMouseButtonPressedImpl(int a_button);
		virtual float GetMouseXImpl();
		virtual float GetMouseYImpl();
		virtual std::pair<float, float> GetMousePosImpl();
	};

}