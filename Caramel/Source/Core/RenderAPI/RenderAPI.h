#pragma once

#include <Core/Core.h>

namespace Caramel
{
	class RenderAPI
	{
	protected:
		friend class GLFW_Window;

		static RenderAPI* Create(WindowRenderAPI api);

		virtual void Initialise(class Window* window, const struct WindowProperties* props) = 0;
		virtual void Shutdown() = 0;
		virtual void RenderFrame() = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual void SetRefreshRate(unsigned int frameRate) = 0;
	};
}


