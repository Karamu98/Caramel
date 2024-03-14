#include "clpch.h"
#include "RenderAPI_OpenGL.h"
#include "Core/Window.h"

#include "Core/RenderAPI/Buffer.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>



void Caramel::RenderAPI_OpenGL::Initialise(Window* window, const WindowProperties* props)
{
	m_window = window->GetNativeWindow<GLFWwindow*>();
	glfwMakeContextCurrent(m_window);
	CL_CORE_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
}

void Caramel::RenderAPI_OpenGL::Shutdown()
{
	CL_CORE_NOT_IMPLEMENTED;
}

void Caramel::RenderAPI_OpenGL::SetVSync(bool enabled)
{
	glfwSwapInterval(enabled);
}

void Caramel::RenderAPI_OpenGL::SetRefreshRate(unsigned int frameRate)
{
	CL_CORE_NOT_IMPLEMENTED;
}

void Caramel::RenderAPI_OpenGL::Clear()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Caramel::RenderAPI_OpenGL::Render()
{

}

void Caramel::RenderAPI_OpenGL::Present()
{
	glfwSwapBuffers(m_window);
}

void Caramel::RenderAPI_OpenGL::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
{
	glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}
