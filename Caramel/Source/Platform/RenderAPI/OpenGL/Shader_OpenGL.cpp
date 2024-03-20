#include "clpch.h"
#include "Shader_OpenGL.h"


#include <glad/glad.h>

namespace Caramel
{
	Shader_OpenGL::Shader_OpenGL(const ShaderParams& params)
	{
		// Read our shaders into the appropriate buffers
		std::string vertexSource = params.vertexSrc; // Get source code for vertex shader.
		std::string fragmentSource = params.fragmentSrc; // Get source code for fragment shader.

		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = (const GLchar*)vertexSource.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.

			CL_CORE_ERROR("Vertex shader failed compilation\n{}", infoLog.data());

			// In this simple program, we'll just leave
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar*)fragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			CL_CORE_ERROR("Fragment shader failed compilation\n{}", infoLog.data());

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_id = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(m_id, vertexShader);
		glAttachShader(m_id, fragmentShader);

		// Link our program
		glLinkProgram(m_id);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_id, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_id, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_id);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.

			CL_CORE_ERROR("Shader failed linking\n{}", infoLog.data());

			// In this simple program, we'll just leave
			return;
		}

		for (auto& uniform : params.Uniforms)
		{
			m_uniformLocations[uniform] = glGetUniformLocation(m_id, uniform.c_str());
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_id, vertexShader);
		glDetachShader(m_id, fragmentShader);
	}

	Shader_OpenGL::~Shader_OpenGL()
	{
		glDeleteProgram(m_id);
	}

	void Shader_OpenGL::Bind()
	{
		glUseProgram(m_id);
	}

	void Shader_OpenGL::Unbind()
	{
		glUseProgram(0);
	}

	void Shader_OpenGL::SetValue(const std::string& name, ShaderDataType type, void* value)
	{
		if (m_uniformLocations.find(name) == m_uniformLocations.end())
		{
			CL_CORE_WARN("Uniform ({0}), is not found in bound shader with ID: {1}", name, m_id);
			return;
		}

		uint32_t targetUniform = m_uniformLocations[name];

		glUseProgram(m_id);	
		switch (type)
		{
		case ShaderDataType::Mat3:		{ glUniformMatrix3fv(targetUniform, 1, GL_TRUE, (float*)value); break; }
		case ShaderDataType::Mat4:		{ glUniformMatrix4fv(targetUniform, 1, GL_TRUE, (float*)value); break; }

		case ShaderDataType::Float:		{ glUniform1f(targetUniform, *(float*)value); break; }
		case ShaderDataType::Float2:	{ glUniform2f(targetUniform, *(float*)value, *((float*)value+1)); break; }
		case ShaderDataType::Float3:	{ glUniform3f(targetUniform, *(float*)value, *((float*)value + 1), *((float*)value + 2)); break; }
		case ShaderDataType::Float4:	{ glUniform4f(targetUniform, *(float*)value, *((float*)value + 1), *((float*)value + 2), *((float*)value + 3)); break; }

		case ShaderDataType::Int:		{ glUniform1i(targetUniform, *(uint32_t*)value); break; }
		case ShaderDataType::Int2:		{ glUniform2i(targetUniform, *(uint32_t*)value, *((uint32_t*)value + 1)); break; }
		case ShaderDataType::Int3:		{ glUniform3i(targetUniform, *(uint32_t*)value, *((uint32_t*)value + 1), *((uint32_t*)value + 2)); break; }
		case ShaderDataType::Int4:		{ glUniform4i(targetUniform, *(uint32_t*)value, *((uint32_t*)value + 1), *((uint32_t*)value + 2), *((uint32_t*)value + 3)); break; }

		}
	}
}

