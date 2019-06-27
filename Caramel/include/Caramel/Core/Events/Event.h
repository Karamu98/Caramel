#pragma once

#include "clpch.h"

namespace Caramel
{
	// FIXME: Events in Caramel are currently blocking.

	// All of our type that an event can be
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	// Event categories for each different event implementation
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

// Macro to give an event an event type (Pass EventType)
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

// Macro to give an event an event category (Pass EventCategory)
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	// Base event class for all events to dirive from
	class Event
	{
	public:
		bool Handled = false;

		// Pure virtual functions to be defined in dirivitives
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); } // Simple debug to test events are actually working

		// Inline as will be used often, to test an events category
		inline bool IsInCategory(EventCategory a_category)
		{
			// Event category flags AND'd with quiried category to see if it's set
			return GetCategoryFlags()& a_category;
		}
	};

	// Created "OnEvent" with the incomming event to attempt to dispatch it to a function
	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event) : m_event(event)
		{
		}

		template<typename T>
		bool Dispatch(EventFn<T> a_function)
		{
			// If the incomming event is equal to our type
			if (m_event.GetEventType() == T::GetStaticType())
			{
				// Attempt to handle the event by calling the function
				m_event.Handled = a_function(*(T*)& m_event);
				return true;
			}
			return false;
		}
	private:
		Event& m_event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}

