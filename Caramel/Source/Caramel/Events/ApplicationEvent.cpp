#include "clpch.h"
#include "ApplicationEvent.h"

namespace Caramel
{
	std::string Caramel::WindowResizeEvent::ToString() const
	{
		std::stringstream ss;
		ss << "WindowResizeEvent (" << m_width << ", " << m_height << ")";
		return ss.str();
	}
}
