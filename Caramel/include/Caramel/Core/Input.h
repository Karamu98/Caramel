#pragma once

namespace Caramel 
{
	// FIXME: Keycodes are only GLFW currently
	class Input
	{
	public:
		static bool IsKeyPressed(int a_keycode) { return s_Instance->IsKeyPressedImpl(a_keycode); }

		inline static bool IsMouseButtonPressed(int a_button) { return s_Instance->IsMouseButtonPressedImpl(a_button); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
		inline static std::pair<float, float> GetMousePos() { return s_Instance->GetMousePosImpl(); }

	protected:
		// Pure virtuals for implementation platform specific (WindowsInput, MacInput etc)
		virtual bool IsKeyPressedImpl(int a_keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(int a_button) = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
		virtual std::pair<float, float> GetMousePosImpl() = 0;

	private:
		static Input* s_Instance;
	};

}