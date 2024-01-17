#pragma once
#include "Core/Events/Event.h"

namespace Caramel
{
	class KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_keyCode; }

		EVENT_CATEGORY(EventCategory::Keyboard | EventCategory::Input)
	protected:
		KeyEvent(int keyCode) : m_keyCode(keyCode) {};

		int m_keyCode;
	};

	class KeyPressEvent : public KeyEvent
	{
	public:
		KeyPressEvent(int keyCode, int repeatCount) : KeyEvent(keyCode), m_repeatCount(repeatCount) {};

		inline int GetRepeatCount() const { return m_repeatCount; }

		std::string ToString() const override;

		EVENT_CLASS_TYPE(KeyPress);

	private:
		int m_repeatCount;
	};

	class KeyReleaseEvent : public KeyEvent
	{
	public:
		KeyReleaseEvent(int keyCode) : KeyEvent(keyCode) {};

		std::string ToString() const override;

		EVENT_CLASS_TYPE(KeyRelease);
	};
}

