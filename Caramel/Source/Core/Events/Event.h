#pragma once
#include "Core/Core.h"

#define BIND_EVENT_FN(x) std::bind(x, this, std::placeholders::_1)

namespace Caramel
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPress, KeyRelease,
		MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll
	};

	enum class EventCategory
	{
		None = 0,
		Application = BIT(0),
		Input = BIT(1),
		Keyboard = BIT(2),
		Mouse = BIT(3),
		MouseButton = BIT(4)
	};
	inline int operator|(const EventCategory& a, const EventCategory& b) { return (int)a | (int)b;}

#define EVENT_CLASS_TYPE(type)	static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override {return GetStaticType();};\
								virtual const char* GetName() const override {return #type;}

#define EVENT_CATEGORY(category) virtual int GetCategoryFlags() const override {return category;}


	class Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); };
		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & (int)category;
		}

		bool Handled = false;
	};

	class EventDispatcher
	{
		template<typename T>
		using EventCallback = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event) : m_event(event) {};

		template<typename T>
		bool Dispatch(EventCallback<T> function)
		{
			if (m_event.GetEventType() == T::GetStaticType())
			{
				m_event.Handled = function(*(T*)&m_event);
				return true;
			}
			return false;
		}

	private:
		Event& m_event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& event)
	{
		return os << event.ToString();
	}

}

