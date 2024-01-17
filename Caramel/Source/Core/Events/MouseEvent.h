#pragma once
#include "Core/Events/Event.h"

namespace Caramel
{
	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(float x, float y) : m_mouseX(x), m_mouseY(y) {};

		inline float GetX() const { return m_mouseX; };
		inline float GetY() const { return m_mouseY; };

		EVENT_CLASS_TYPE(MouseMove);
		EVENT_CATEGORY(EventCategory::Mouse | EventCategory::Input);

	private:
		float m_mouseX;
		float m_mouseY;
	};

	class MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(float xOff, float yOff) : m_xOffset(xOff), m_yOffset(yOff) {};

		inline float GetX() const { return m_xOffset; }
		inline float GetY() const { return m_yOffset; }

		EVENT_CLASS_TYPE(MouseScroll);
		EVENT_CATEGORY(EventCategory::Mouse | EventCategory::Input);

	private:
		float m_xOffset;
		float m_yOffset;
	};

	class MouseButtonEvent : public Event
	{
	public:
		inline int GetButton() const { return m_buttonCode; }

	protected:
		MouseButtonEvent(int code) : m_buttonCode(code) {};

		EVENT_CATEGORY(EventCategory::Mouse | EventCategory::Input);

		int m_buttonCode;
	};

	class MouseButtonPressEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressEvent(int code) : MouseButtonEvent(code) {}

		std::string ToString() const override;

		EVENT_CLASS_TYPE(MouseButtonPress);
	};

	class MouseButtonReleaseEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleaseEvent(int code) : MouseButtonEvent(code) {}

		std::string ToString() const override;

		EVENT_CLASS_TYPE(MouseButtonRelease);
	};
}


