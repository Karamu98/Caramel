#include "clpch.h"
#include "KeyEvents.h"

namespace Caramel
{
	std::string KeyPressEvent::ToString() const
	{
		std::stringstream ss;
		ss << "KeyPressEvent: " << m_keyCode << " (" << m_repeatCount << " repeats)";
		return ss.str();
	}

	std::string KeyReleaseEvent::ToString() const
	{
		std::stringstream ss;
		ss << "KeyReleaseEvent: " << m_keyCode;
		return ss.str();
	}
}
