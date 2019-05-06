#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <glad/glad.h>

std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

Log::Log()
{

}

Log::~Log()
{
}

void Log::Init()
{
	spdlog::set_pattern("%^[%T] %n: %v%$");
	s_CoreLogger = spdlog::stdout_color_mt("Caramel");
	s_CoreLogger->set_level(spdlog::level::trace);
	s_ClientLogger = spdlog::stdout_color_mt("Game");
	s_ClientLogger->set_level(spdlog::level::trace);
}

void Log::GetOpenGLErrors(const char* a_file, int a_line)
{
	GLenum errorNumber(glGetError());

	while (errorNumber != GL_NO_ERROR) 
	{
		std::string error;

		switch (errorNumber) 
		{
		case GL_INVALID_OPERATION:				 error = "INVALID_OPERATION";			   break;
		case GL_INVALID_ENUM:					 error = "INVALID_ENUM";				   break;
		case GL_INVALID_VALUE:					 error = "INVALID_VALUE";				   break;
		case GL_OUT_OF_MEMORY:					 error = "OUT_OF_MEMORY";				   break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:   error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}

		s_CoreLogger->error("OpenGL: \n" + error + "\nIn file: " + std::string(a_file) + " (" + std::to_string(a_line) + ")");
		errorNumber = glGetError();
	}
}
