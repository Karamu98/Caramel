#include "clpch.h"
#include "MouseEvent.h"

namespace Caramel
{
	std::string MouseButtonPressEvent::ToString() const
	{
		std::stringstream ss;
		ss << "MouseButtonPressEvent: " << m_buttonCode;
		return ss.str();
	}

	std::string MouseButtonReleaseEvent::ToString() const
	{
		std::stringstream ss;
		ss << "MouseButtonReleaseEvent: " << m_buttonCode;
		return ss.str();
	}

}
