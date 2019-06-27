#pragma once

#include "Caramel/Core/Input.h"

namespace Caramel {

	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode);

		virtual bool IsMouseButtonPressedImpl(int button);
		virtual float GetMouseXImpl();
		virtual float GetMouseYImpl();
	};

}