#include "clpch.h"
#include "Shader.h"
#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp>


namespace Caramel
{
	// If this is defined, we don't want to delete shaders to allow for easy debugging
#ifndef CL_DEBUG

#define DeleteShader(...) glDeleteShader(__VA_ARGS__)

#else

#define DeleteShader

#endif


	Shader::Shader(const std::string& a_sourcePath) : m_isValid(false)
	{
		m_shaderPath = a_sourcePath;
	}

	std::string Shader::StringFromShaderType(const unsigned int a_type)
	{
		if (a_type == GL_VERTEX_SHADER)
			return "vertex";
		if (a_type == GL_FRAGMENT_SHADER)
			return "fragment";

		assert(false, "Unknown shader type");
		return 0;
	}

	unsigned int Shader::ShaderTypeFromString(const std::string& a_type)
	{
		if (a_type == "vertex")
			return GL_VERTEX_SHADER;
		if (a_type == "fragment" || a_type == "pixel")
			return GL_FRAGMENT_SHADER;

		assert(false, "Unknown shader type");
		return 0;
	}

	bool Shader::Compile(const std::unordered_map<unsigned int, std::string>& a_sources)
	{
		GLuint program = glCreateProgram();
		std::vector<GLenum> glShaderIDs(a_sources.size());
		m_isValid = false;

		// Loop through all our sources and compile them
		for (auto& kv : a_sources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			// Create the shader
			GLuint shader = glCreateShader(type);

			// Convert the source and give it to OpenGL
			const GLchar* sourceStr = source.c_str();
			glShaderSource(shader, 1, &sourceStr, 0);

			// Compile
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				DeleteShader(shader);

				std::cout << StringFromShaderType(type) << " shader failed to compile\n" << infoLog.data() << std::endl;
				return false;
			}

			// Finally attach the shader to the program
			glAttachShader(program, shader);
			glShaderIDs.push_back(shader);
		}

		// Now try link the program
		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
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

			std::cout << "Program failed to Link\n" << infoLog.data() << std::endl;
			return false;
		}

		// Detach all our shaders
		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
			DeleteShader(id);
		}

		// If we successfully created a shader, assign ID and validate
		m_shaderProgram = program;
		m_isValid = true;

		return true;
	}

	bool Shader::LocationValid(unsigned int& a_loc, const std::string& a_name, bool a_logErrors)
	{
		if (a_loc == -1)
		{
			if (a_logErrors)
			{
				std::cout << "Unable to find location of '" << a_name << "' in shader\n";
			}
			return false;
		}
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
				std::cout << "Syntax error when looking for shader type\n";
				return nullptr;
			}

			// Find where our type string starts and grab it
			size_t begin = pos + tokenLength + 1; // +1 for space
			std::string type = file->substr(begin, eol - begin);
			if (ShaderTypeFromString(type) == 0)
			{
				std::cout << "Invalid shader type\n";
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
		std::string fullPath = Utility::GetWorkingDir() + a_shaderPath;
		std::shared_ptr< std::unordered_map<unsigned int, std::string>> sources = Preprocess(fullPath);

		if (sources == nullptr)
		{
			return nullptr;
		}

		// Now that we have all the source files separated, give them to OpenGL
		std::shared_ptr<Shader> newShader = std::make_shared<Shader>(fullPath);

		// Now try create a shader with our sorted sources
		if (newShader->Compile(*sources))
		{
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
		unsigned int loc = glGetUniformLocation(m_shaderProgram, a_name.c_str());

		if (LocationValid(loc, a_name, a_logErrors))
		{
			glUniform1i(loc, (int)a_value);
		}
	}

	void Shader::SetInt(const std::string& a_name, int a_value, bool a_logErrors)
	{
		unsigned int loc = glGetUniformLocation(m_shaderProgram, a_name.c_str());

		if (LocationValid(loc, a_name, a_logErrors))
		{
			glUniform1i(loc, (int)a_value);
		}
	}

	void Shader::SetFloat(const std::string& a_name, float a_value, bool a_logErrors)
	{
		unsigned int loc = glGetUniformLocation(m_shaderProgram, a_name.c_str());

		if (LocationValid(loc, a_name, a_logErrors))
		{
			glUniform1f(loc, a_value);
		}
	}

	void Shader::SetVec4(const std::string& a_name, glm::vec4 a_value, bool a_logErrors)
	{
		unsigned int loc = glGetUniformLocation(m_shaderProgram, a_name.c_str());

		if (LocationValid(loc, a_name, a_logErrors))
		{
			float* valArray = glm::value_ptr(a_value);

			glUniform4f(loc, valArray[0], valArray[1], valArray[2], valArray[3]);
		}
	}

	void Shader::SetVec3(const std::string& a_name, glm::vec3 a_value, bool a_logErrors)
	{
		unsigned int loc = glGetUniformLocation(m_shaderProgram, a_name.c_str());

		if (LocationValid(loc, a_name, a_logErrors))
		{
			float* valArray = glm::value_ptr(a_value);
			glUniform3f(loc, valArray[0], valArray[1], valArray[2]);
		}
	}

	void Shader::SetVec2(const std::string& a_name, glm::vec2 a_value, bool a_logErrors)
	{
		unsigned int loc = glGetUniformLocation(m_shaderProgram, a_name.c_str());

		if (LocationValid(loc, a_name, a_logErrors))
		{
			float* valArray = glm::value_ptr(a_value);
			glUniform2f(loc, valArray[0], valArray[1]);
		}
	}

	void Shader::SetMat4(const std::string& a_name, glm::mat4 a_value, bool a_logErrors)
	{
		unsigned int loc = glGetUniformLocation(m_shaderProgram, a_name.c_str());

		if (LocationValid(loc, a_name, a_logErrors))
		{
			glUniformMatrix4fv(loc, 1, false, glm::value_ptr(a_value));
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