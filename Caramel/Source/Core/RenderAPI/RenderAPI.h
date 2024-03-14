#pragma once

#include <Core/Core.h>


namespace Caramel
{
	class VertexArray;

	class RenderAPI
	{
	public:
		static RenderAPI* Create(WindowRenderAPI api);

		virtual void Initialise(class Window* window, const struct WindowProperties* props) = 0;
		virtual void Shutdown() = 0;

		virtual void Clear() = 0;
		virtual void Render() = 0;
		virtual void Present() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual void SetRefreshRate(unsigned int frameRate) = 0;
	};
}


