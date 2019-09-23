#include "clpch.h"
#include "Shader.h"
#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp>
#include "Core/Log.h"
#include "Core/Utilities.h"


namespace Caramel
{
	// If this is defined, we don't want to delete shaders to allow for easy debugging
#ifndef CL_DEBUG

#define DeleteShader(...) glDeleteShader(__VA_ARGS__)

#else

#define DeleteShader

#endif

	std::unordered_map<std::string, std::weak_ptr<Shader>> Shader::s_shaderLibrary;

	Shader::Shader(const std::string& a_sourcePath) : m_isValid(false)
	{
		m_shaderPath = a_sourcePath;
	}

	unsigned int Shader::GetUniformLocation(const std::string& a_name) const
	{
		if (m_uniformCache.find(a_name) != m_uniformCache.end())
		{
			return m_uniformCache[a_name];
		}
		
		unsigned int loc = glGetUniformLocation(m_shaderProgram, a_name.c_str());

		m_uniformCache[a_name] = loc;
		return loc;
	}

	std::string Shader::StringFromShaderType(const unsigned int a_type)
	{
		if (a_type == GL_VERTEX_SHADER)
			return "vertex";
		if (a_type == GL_FRAGMENT_SHADER)
			return "fragment";

		CL_CORE_ASSERT(false, "Unknown shader type");
		return "";
	}

	unsigned int Shader::ShaderTypeFromString(const std::string& a_type)
	{
		if (a_type == "vertex")
			return GL_VERTEX_SHADER;
		if (a_type == "fragment" || a_type == "pixel")
			return GL_FRAGMENT_SHADER;

		CL_CORE_ASSERT(false, "Unknown shader type");
		return 0;
	}

	bool Shader::Compile(const std::unordered_map<unsigned int, std::string>& a_sources)
	{
		GLuint program = glCreateProgram();
		Utility::GetGLErrors();
		std::vector<unsigned int> glShaderIDs; // Stack alloc for performance over memory (20bytes max)
		glShaderIDs.reserve(5);
		m_isValid = false;

		// Loop through all our sources and compile them
		for (auto& kv : a_sources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			// Create the shader
			GLuint shader = glCreateShader(type);
			Utility::GetGLErrors();

			// Convert the source and give it to OpenGL
			const GLchar* sourceStr = source.c_str();
			glShaderSource(shader, 1, &sourceStr, 0);
			Utility::GetGLErrors();

			// Compile
			glCompileShader(shader);
			Utility::GetGLErrors();

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				DeleteShader(shader);

				CL_CORE_ERROR("{0} shader failed to compile\n{1}\n", StringFromShaderType(type), infoLog.data());
				return false;
			}

			// Finally attach the shader to the program
			glAttachShader(program, shader);
			Utility::GetGLErrors();
			glShaderIDs.push_back(shader);
		}

		// Now try link the program
		glLinkProgram(program);
		Utility::GetGLErrors();

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		Utility::GetGLErrors();
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);

			for (auto id : glShaderIDs)
			{
				glDeleteShader(id);
			}
			CL_CORE_ERROR("Program failed to link\n{0}\n", infoLog.data());
			return false;
		}

		// Detach all our shaders
		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
			Utility::GetGLErrors();
			DeleteShader(id);
			Utility::GetGLErrors();
		}

		// If we successfully created a shader, assign ID and validate
		m_shaderProgram = program;
		m_isValid = true;
		m_uniformCache.clear(); // Clear to grab new uniforms
		Utility::GetGLErrors();
		return true;
	}

	std::shared_ptr<std::unordered_map<unsigned int, std::string>> Shader::Preprocess(const std::string& a_shaderPath)
	{
		std::shared_ptr<std::unordered_map<unsigned int, std::string>> sources = std::make_shared<std::unordered_map<unsigned int, std::string>>();

		// Get file contents
		std::shared_ptr<std::string> file = Utility::ReadFile(a_shaderPath);

		// If we opened a file, process it
		if (!file)
		{
			return nullptr;
		}

		const char* typeToken = "#type";
		size_t tokenLength = strlen(typeToken);
		size_t pos = file->find(typeToken, 0);


		// While we read through the file
		while (pos != std::string::npos)
		{
			// End of line
			size_t eol = file->find_first_of("\r\n", pos);
			if (eol == std::string::npos)
			{
				CL_CORE_ERROR("Syntax error when looking for shader type");
				return nullptr;
			}

			// Find where our type string starts and grab it
			size_t begin = pos + tokenLength + 1; // +1 for space
			std::string type = file->substr(begin, eol - begin);
			if (ShaderTypeFromString(type) == 0)
			{
				CL_CORE_ERROR("Invalid shader type");
				return nullptr;
			}

			// Get the position of the next line
			size_t nextLinePos = file->find_first_not_of("\r\n", eol);
			pos = file->find(typeToken, nextLinePos); // Find the next token after our tag

			// Read from below the tag we found to either the end of file or the next tag
			(*sources)[ShaderTypeFromString(type)] = file->substr(nextLinePos, pos - (nextLinePos == std::string::npos ? file->size() - 1 : nextLinePos));
		}

		return sources;
	}

	std::shared_ptr<Shader> Shader::CreateShader(const std::string& a_shaderPath)
	{
		std::shared_ptr<Shader> newShader;
		// If the shader is tracked
		if (s_shaderLibrary.find(a_shaderPath) != s_shaderLibrary.end())
		{
			// And if the shader is still loaded, return the shader
			if (newShader = s_shaderLibrary[a_shaderPath].lock())
			{
				return newShader;
			}
		}

		std::shared_ptr< std::unordered_map<unsigned int, std::string>> sources = Preprocess(a_shaderPath);

		if (sources == nullptr)
		{
			return nullptr;
		}

		// Now that we have all the source files separated, give them to OpenGL
		newShader = std::make_shared<Shader>(a_shaderPath);

		// Now try create a shader with our sorted sources
		if (newShader->Compile(*sources))
		{
			s_shaderLibrary[a_shaderPath] = newShader;
			return newShader;
		}
		else
		{
			return nullptr;
		}
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_shaderProgram);
	}

	bool Shader::Bind()
	{
		if (m_isValid)
		{
			glUseProgram(m_shaderProgram);
			return true;
		}

		return false;
	}

	void Shader::Unbind()
	{
		glUseProgram(0);
	}

	void Shader::SetBool(const std::string& a_name, bool a_value, bool a_logErrors)
	{
		unsigned int loc = GetUniformLocation(a_name);

		if (loc == -1 && a_logErrors)
		{
			CL_CORE_WARN("{0} not found in shader", a_name);
		}
		else if(loc != -1)
		{
			glUniform1i(loc, (int)a_value);
		}
	}

	void Shader::SetInt(const std::string& a_name, int a_value, bool a_logErrors)
	{
		unsigned int loc = GetUniformLocation(a_name);

		if (loc == -1 && a_logErrors)
		{
			CL_CORE_WARN("{0} not found in shader", a_name);
		}
		else if (loc != -1)
		{
			glUniform1i(loc, (int)a_value);
		}
	}

	void Shader::SetFloat(const std::string& a_name, float a_value, bool a_logErrors)
	{
		unsigned int loc = GetUniformLocation(a_name);

		if (loc == -1 && a_logErrors)
		{
			CL_CORE_WARN("{0} not found in shader", a_name);
		}
		else if (loc != -1)
		{
			glUniform1f(loc, a_value);
		}
	}

	void Shader::SetVec4(const std::string& a_name, glm::vec4& a_value, bool a_logErrors)
	{
		unsigned int loc = GetUniformLocation(a_name);

		if (loc == -1 && a_logErrors)
		{
			CL_CORE_WARN("{0} not found in shader", a_name);
		}
		else if (loc != -1)
		{
			float* valArray = glm::value_ptr(a_value);

			glUniform4f(loc, valArray[0], valArray[1], valArray[2], valArray[3]);
		}
	}

	void Shader::SetVec3(const std::string& a_name, glm::vec3& a_value, bool a_logErrors)
	{
		unsigned int loc = GetUniformLocation(a_name);

		if (loc == -1 && a_logErrors)
		{
			CL_CORE_WARN("{0} not found in shader", a_name);
		}
		else if (loc != -1)
		{
			float* valArray = glm::value_ptr(a_value);
			glUniform3f(loc, valArray[0], valArray[1], valArray[2]);
		}
	}

	void Shader::SetVec3(const std::string& a_name, glm::detail::_swizzle<3, float, glm::packed_highp, glm::tvec3, 0, 1, 2, -1> & a_value, bool a_logErrors)
	{
		unsigned int loc = GetUniformLocation(a_name);

		if (loc == -1 && a_logErrors)
		{
			CL_CORE_WARN("{0} not found in shader", a_name);
		}
		else if (loc != -1)
		{
			glUniform3f(loc, a_value[0], a_value[1], a_value[2]);
		}
	}

	void Shader::SetVec2(const std::string& a_name, glm::vec2& a_value, bool a_logErrors)
	{
		unsigned int loc = GetUniformLocation(a_name);

		if (loc == -1 && a_logErrors)
		{
			CL_CORE_WARN("{0} not found in shader", a_name);
		}
		else if (loc != -1)
		{
			float* valArray = glm::value_ptr(a_value);
			glUniform2f(loc, valArray[0], valArray[1]);
		}
	}

	void Shader::SetMat4(const std::string& a_name, glm::mat4& a_value, bool a_logErrors)
	{
		unsigned int loc = GetUniformLocation(a_name);

		if (loc == -1 && a_logErrors)
		{
			CL_CORE_WARN("{0} not found in shader", a_name);
		}
		else if (loc != -1)
		{
			glUniformMatrix4fv(loc, 1, false, glm::value_ptr(a_value));
		}
	}

	void Shader::SetMat4(const std::string& a_name, Transform& a_value, bool a_logErrors)
	{
		unsigned int loc = GetUniformLocation(a_name);

		if (loc == -1 && a_logErrors)
		{
			CL_CORE_WARN("{0} not found in shader", a_name);
		}
		else if (loc != -1)
		{
			glUniformMatrix4fv(loc, 1, false, glm::value_ptr(*a_value.GetMatrix()));
		}
	}

	void Shader::Recompile()
	{
		std::shared_ptr< std::unordered_map<unsigned int, std::string>> sources = Preprocess(m_shaderPath);

		if (sources == nullptr)
		{
			return;
		}

		glDeleteProgram(m_shaderProgram);
		Compile(*sources);
	}
}