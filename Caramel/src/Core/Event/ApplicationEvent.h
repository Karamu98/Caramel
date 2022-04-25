#pragma once

#include "Event.h"


namespace Caramel 
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height, bool isFullscreen) : m_Width(width), m_Height(height), m_fullscreen(isFullscreen) {}

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }
		bool IsFullscreen() const{ return m_fullscreen; }

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
		bool m_fullscreen;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowMoveEvent : public Event
	{
	public:
		WindowMoveEvent(int newX, int newY) : m_newX(newX), m_newY(newY) {}

		int GetX() const { return m_newX; }
		int GetY() const { return m_newY; }

		EVENT_CLASS_TYPE(WindowMove)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		int m_newX, m_newY;
	};

	class WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent(bool isFocused) :  m_isFocused(isFocused) {}

		bool IsFocused() const { return m_isFocused; }

		EVENT_CLASS_TYPE(WindowFocus)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		bool m_isFocused;
	};
}