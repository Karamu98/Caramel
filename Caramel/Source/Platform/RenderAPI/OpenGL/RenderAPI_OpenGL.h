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
		void Initialise(Window* window, const WindowProperties* props) override;
		void Shutdown() override;
		void SetVSync(bool enabled) override;
		void SetRefreshRate(unsigned int frameRate) override;

	private:
		GLFWwindow* m_window;

		// Inherited via RenderAPI
		void Clear() override;
		void Render() override;
		void Present() override;

		// Inherited via RenderAPI
		void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
	};
}


