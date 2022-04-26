#pragma once

namespace Caramel
{
	enum class WindowManager
	{
		GLFW,
		Invalid
	};

	enum class WindowRenderer
	{
		OpenGL,
		Vulkan,
		DirectX12,
		Invalid
	};

	enum class RendererFunctionLoader
	{
		GLAD,
		Invalid
	};
}

#define CL_BIND(fn) std::bind(&##fn, this, std::placeholders::_1)