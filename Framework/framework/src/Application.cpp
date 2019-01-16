#include "Application.h"
#include "Utilities.h"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>


#define BUILD_VERSION_MAJOR 1
#define BUILD_VERSION_MINOR 0
#define BULID_VERSION_REVISION 1

#ifdef __GL_DEBUG__
void APIENTRY glErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg, const void *data)
{
	char *strSource = "0";
	char *strType = strSource;
	char *strSeverity = strSource;
	switch (source) {
	case GL_DEBUG_SOURCE_API:     strSource = "API";						break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:       strSource = "WINDOWS";		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:     strSource = "SHADER COMP.";	break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:         strSource = "3RD PARTY";		break;
	case GL_DEBUG_SOURCE_APPLICATION:         strSource = "APP";			break;
	case GL_DEBUG_SOURCE_OTHER:               strSource = "OTHER";			break;
	}
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:		        strType = "ERROR";				break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:   strType = "Deprecated";		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:   strType = "Undefined";			break;
	case GL_DEBUG_TYPE_PORTABILITY:           strType = "Portability";		break;
	case GL_DEBUG_TYPE_PERFORMANCE:           strType = "Performance";		break;
	case GL_DEBUG_TYPE_OTHER:                 strType = "Other";			break;
	}
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:	        strSeverity = "High";			break;
	case GL_DEBUG_SEVERITY_MEDIUM:          strSeverity = "Medium";			break;
	case GL_DEBUG_SEVERITY_LOW:             strSeverity = "Low";			break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:	strSeverity = "Notification";	break;
	}
	static int errNum = 0;
	std::fstream file;
	file.open("GL_Error.log", std::ios_base::out | std::ios_base::app);

	file << errNum << ": GL Error Callback: " << "\n Error Source: " << strSource << "\n Error Type: " << strType << "\n Severity: " << strSeverity << "\n Message: " << msg << std::endl;
	file.close();
	++errNum;
}
#endif

bool Application::create(const char* a_name, int a_width, int a_height, bool a_bFullscreen )
{
	// initialise glfw systems
	if (glfwInit() != GL_TRUE)
	{
		std::cout << "Unable to initialize GLFW" << std::endl;
		return false;
	}
	
	// create a windowed mode window and its OpenGL context
	m_window = glfwCreateWindow(a_width, a_height, a_name, ( a_bFullscreen ? glfwGetPrimaryMonitor() : nullptr), nullptr);
	if (m_window == nullptr)
	{
		std::cout << "Unable to create a GLFW Window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(m_window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(m_window, [](GLFWwindow*, int w, int h)
	{
		glViewport(0, 0, w, h);
#ifdef __GL_DEBUG__
		glDebugMessageCallback(glErrorCallback, NULL);
#endif
	});
	
	std::cout << "UG Framework Version " << BUILD_VERSION_MAJOR << "." << BUILD_VERSION_MINOR << "." << BULID_VERSION_REVISION << std::endl;

	int major = glfwGetWindowAttrib(m_window, GLFW_CONTEXT_VERSION_MAJOR);
	int minor = glfwGetWindowAttrib(m_window, GLFW_CONTEXT_VERSION_MINOR);
	int revision = glfwGetWindowAttrib(m_window, GLFW_CONTEXT_REVISION);
	std::cout << "OpenGL Version " << major << "." << minor << "." << revision << std::endl;


		bool result = onCreate();
	if (result == false)
	{
		glfwTerminate();
	}
	return result;
}

void Application::run(const char* a_name, int a_width, int a_height, bool a_bFullscreen)
{
	if (create(a_name, a_width, a_height, a_bFullscreen))
	{
		Utility::resetTimer();
		m_running = true;
		do
		{
			float deltaTime = Utility::tickTimer();

			Update(deltaTime);

			Draw();

			glfwSwapBuffers(m_window);
			glfwPollEvents();

		} while (m_running == true && glfwWindowShouldClose(m_window) == 0);

		Destroy();
	}

	glfwDestroyWindow(m_window);
	glfwTerminate();
}