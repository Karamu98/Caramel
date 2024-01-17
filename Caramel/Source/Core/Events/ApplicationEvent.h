#pragma once
#include "Core/Events/Event.h"

namespace Caramel
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int w, unsigned int h) : m_width(w), m_height(h) {};

		inline unsigned int GetWidth() const { return m_width; }
		inline unsigned int GetHeight() const { return m_height; }

		std::string ToString() const override;

		EVENT_CLASS_TYPE(WindowResize);
		EVENT_CATEGORY((int)EventCategory::Application)

	private:
		unsigned int m_width;
		unsigned int m_height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {};

		EVENT_CLASS_TYPE(WindowClose);
		EVENT_CATEGORY((int)EventCategory::Application)
	};
}