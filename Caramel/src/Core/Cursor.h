#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

struct GLFWwindow;

namespace Caramel
{
	class Cursor
	{
	public:
		static glm::tvec2<double> GetPosition();
	};
}