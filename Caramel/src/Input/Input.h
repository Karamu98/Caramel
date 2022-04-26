#pragma once

#include "KeyCodes.h"
#include "Core/Defines.h"

namespace Caramel
{
	class Input
	{
	public:
		static std::shared_ptr<Input> CreateInputManager(WindowManager manager, std::shared_ptr<class Window> inputWindow);

		static bool GetKeyDown(KeyCode keyCode) { return sm_instance->GetKeyDownImpl(keyCode); };
		static bool GetKeyUp(KeyCode keyCode) { return sm_instance->GetKeyUpImpl(keyCode); };
		static bool GetKey(KeyCode keyCode) { return sm_instance->GetKeyImpl(keyCode); };
		static bool GetMouseButton(MouseButton button) { return sm_instance->GetMouseButtonImpl(button); };
		static bool GetMouseButtonDown(MouseButton button) { return sm_instance->GetMouseButtonDownImpl(button); };
		static bool GetMouseButtonUp(MouseButton button) { return sm_instance->GetMouseButtonUpImpl(button); };
		static bool GetMouseButton(int button) { return sm_instance->GetMouseButtonImpl((MouseButton)button); };
		static bool GetMouseButtonDown(int button) { return sm_instance->GetMouseButtonDownImpl((MouseButton)button); };
		static bool GetMouseButtonUp(int button) { return sm_instance->GetMouseButtonUpImpl((MouseButton)button); };
		static std::pair<float, float> GetMousePosition() { return sm_instance->GetMousePositionImpl(); }
		static float GetMousePositionX() { return sm_instance->GetMousePositionImpl().first; }
		static float GetMousePositionY() { return sm_instance->GetMousePositionImpl().second; }

	protected:
		virtual bool GetKeyDownImpl(KeyCode keyCode) = 0;
		virtual bool GetKeyUpImpl(KeyCode keyCode) = 0;
		virtual bool GetKeyImpl(KeyCode keyCode) = 0;
		virtual bool GetMouseButtonImpl(MouseButton button) = 0;
		virtual bool GetMouseButtonDownImpl(MouseButton button) = 0;
		virtual bool GetMouseButtonUpImpl(MouseButton button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;

		static Input* sm_instance;
	};
}
