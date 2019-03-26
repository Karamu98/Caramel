#include "..\include\Shader.h"
#include "Utilities.h"
#include "Log.h"
#include "gl_core_4_4.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Renderer.h"


// If this is defined, we don't want to delete shaders to allow for easy debugging
#ifndef CL_DEBUG

#define DeleteShader(...) glDeleteShader(__VA_ARGS__)

#else

#define DeleteShader

#endif



Shader::Shader(const char * a_vertexPath, const char * a_fragPath, const char * a_geometryShader, const char * a_tessalationShader)
{
	// These will hold the source of our files if we specify them
	unsigned char* vertSource;
	unsigned char* fragSource;
	unsigned char* geoSource = nullptr;
	unsigned char* tessSource = nullptr;

	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int geometryShader = 0;
	unsigned int tessalationShader = 0;

	bool bValidGeo = false;
	bool bValidTess = false;

#pragma region NullChecking
	if (a_vertexPath == nullptr)
	{
		CL_CORE_ERROR("A vertex shader must be supplied, cannot be nullptr.");
		return;
	}
	else
	{
		vertSource = Utility::fileToBuffer(a_vertexPath);
		if (vertSource == nullptr)
		{
			CL_CORE_ERROR("A vertex shader could not be loaded at ", a_vertexPath);
			return;
		}
	}

	if (a_fragPath == nullptr)
	{
		CL_CORE_ERROR("A fragment shader must be supplied, cannot be nullptr.");
		return;
	}
	else
	{
		fragSource = Utility::fileToBuffer(a_fragPath);
		if (fragSource == nullptr)
		{
			CL_CORE_ERROR("A fragment shader could not be loaded at ", fragSource);
			return;
		}
	}

	if (a_geometryShader != nullptr)
	{
		geoSource = Utility::fileToBuffer(a_geometryShader);
		if (geoSource == nullptr)
		{
			CL_CORE_ERROR("A geometry shader could not be loaded at ", geoSource);
			return;
		}
	}

	if (a_tessalationShader != nullptr)
	{
		tessSource = Utility::fileToBuffer(a_tessalationShader);
		if (tessSource == nullptr)
		{
			CL_CORE_ERROR("A tessalation shader could not be loaded at ", tessSource);
			return;
		}
	}

#pragma endregion

	// Vertex shader setup
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const char**)&vertSource, NULL);
	glCompileShader(vertexShader);
	if (!VerifyShader(vertexShader))
	{
		// Cleanup
		return;
	}

	// Fragment shader setup
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const char**)&fragSource, NULL);
	glCompileShader(fragmentShader);
	if (!VerifyShader(fragmentShader))
	{
		// Cleanup
		return;
	}

	// Geometry shader setup
	if (geoSource != nullptr)
	{
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, (const char**)&geoSource, NULL);
		glCompileShader(geometryShader);
		if (!VerifyShader(geometryShader))
		{
			// Cleanup
			return;
		}
		else
		{
			bValidGeo = true;
		}
	}

	// Tessalation shader setup
	if (tessSource != nullptr)
	{
		geometryShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(tessalationShader, 1, (const char**)&tessSource, NULL);
		glCompileShader(tessalationShader);
		if (!VerifyShader(tessalationShader))
		{
			// Cleanup
			return;
		}
		else
		{
			bValidTess = true;
		}
	}

	m_shaderProgram = glCreateProgram();


	glAttachShader(m_shaderProgram, vertexShader);
	if (bValidGeo)
	{
		glAttachShader(m_shaderProgram, geometryShader);
	}
	if (bValidTess)
	{
		glAttachShader(m_shaderProgram, tessalationShader);
	}
	glAttachShader(m_shaderProgram, fragmentShader);

	glLinkProgram(m_shaderProgram);


	int success = GL_FALSE;
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(m_shaderProgram, infoLogLength, 0, infoLog);

		std::string log = std::string(infoLog);

		CL_CORE_ERROR("Failed linking program.\n" + log);
		delete[] infoLog;
	}

	DeleteShader(vertexShader);
	DeleteShader(fragmentShader);

	if (bValidGeo)
	{
		DeleteShader(geometryShader);
	}

	if (bValidTess)
	{
		DeleteShader(tessalationShader);
	}

}


Shader::~Shader()
{
	glDeleteProgram(m_shaderProgram);
}

void Shader::Bind()
{
	glUseProgram(m_shaderProgram);
}

void Shader::Unbind()
{
	glUseProgram(0);
}

void Shader::RegisterRenderable(MeshFilter* a_other)
{
	m_toRender.push_back(a_other);
}

void Shader::Draw()
{
	for (MeshFilter* mesh : m_toRender)
	{
		mesh->Draw(this);
	}
}

void Shader::UnregisterRenderable(MeshFilter * a_other)
{
	ptrdiff_t old = std::find(m_toRender.begin(), m_toRender.end(), a_other) - m_toRender.begin();

	m_toRender.erase(m_toRender.begin() + old);
}

void Shader::SetBool(const std::string& a_name, bool a_value)
{
	glUniform1i(glGetUniformLocation(m_shaderProgram, a_name.c_str()), (int)a_value);
}

void Shader::SetInt(const std::string& a_name, int a_value)
{
	glUniform1i(glGetUniformLocation(m_shaderProgram, a_name.c_str()), a_value);
}

void Shader::SetFloat(const std::string& a_name, float a_value)
{
	glUniform1f(glGetUniformLocation(m_shaderProgram, a_name.c_str()), a_value);
}

void Shader::SetVec4(const std::string & a_name, glm::vec4 a_value)
{
	float* valArray = glm::value_ptr(a_value);
	glUniform4f(glGetUniformLocation(m_shaderProgram, a_name.c_str()),valArray[0], valArray[1], valArray[2], valArray[3]);
}

void Shader::SetVec3(const std::string & a_name, glm::vec3 a_value)
{
	float* valArray = glm::value_ptr(a_value);
	glUniform3f(glGetUniformLocation(m_shaderProgram, a_name.c_str()), valArray[0], valArray[1], valArray[2]);
}

void Shader::SetMat4(const std::string & a_name, glm::mat4 a_value)
{
	unsigned int loc = glGetUniformLocation(m_shaderProgram, a_name.c_str());

	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(a_value));
}

bool Shader::VerifyShader(unsigned int & a_shaderHandle)
{
	int success = GL_FALSE;

	// Check if it compiled correctly
	glGetShaderiv(a_shaderHandle, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetShaderiv(a_shaderHandle, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetShaderInfoLog(a_shaderHandle, infoLogLength, 0, infoLog);

		std::string log = std::string(infoLog);

		CL_CORE_ERROR("Failed to compile shader.\n" + log);
		delete[] infoLog;


		return false;
	}
	else
	{
		return true;
	}	
}
