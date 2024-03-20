#pragma once

#include <Core/RenderAPI/RenderAPI.h>


struct GLFWwindow;

namespace Caramel
{
	// USES GLFW CURRENTLY
	class RenderAPI_OpenGL : public RenderAPI
	{
	protected:
		// Inherited via RenderAPI
		virtual void Initialise(Window* window, const WindowProperties* props) override;
		virtual void Shutdown() override;
		virtual void SetVSync(bool enabled) override;
		virtual void SetRefreshRate(unsigned int frameRate) override;

	private:
		GLFWwindow* m_window;

		// Inherited via RenderAPI
		virtual void Clear() override;
		virtual void Render() override;
		virtual void Present() override;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
		virtual void DrawArray(const std::shared_ptr<VertexArray>& vertexArray) override;
	};
}


