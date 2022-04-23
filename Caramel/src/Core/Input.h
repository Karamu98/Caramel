#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

struct GLFWwindow;

namespace Caramel
{
	class Input
	{
	public:
		static bool GetKeyDown(char character);
		static bool GetKeyUp(char character);
		static bool GetKey(char character);

		static bool GetMouseButton(int button);
		static bool GetMouseButtonDown(int button);
		static bool GetMouseButtonUp(int button);
	};
}
