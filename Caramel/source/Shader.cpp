#include "..\include\Shader.h"
#include "Utilities.h"
#include "Log.h"
#include "gl_core_4_4.h"


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
	unsigned char* geoSource;
	unsigned char* tessSource;

	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int geometryShader;
	unsigned int tessalationShader;

	bool bValidGeo = false;
	bool bValidTess = false;

#pragma region NullChecking
	if (a_vertexPath == nullptr)
	{
		CL_CORE_ERROR("A vertex shader must be supplied, cannot be nullptr.");
	}
	else
	{
		vertSource = Utility::fileToBuffer(a_vertexPath);
	}

	if (a_fragPath == nullptr)
	{
		CL_CORE_ERROR("A fragment shader must be supplied, cannot be nullptr.");
	}
	else
	{
		fragSource = Utility::fileToBuffer(a_fragPath);
	}

	if (a_geometryShader != nullptr)
	{
		geoSource = Utility::fileToBuffer(a_geometryShader);
	}

	if (a_tessalationShader != nullptr)
	{
		tessSource = Utility::fileToBuffer(a_tessalationShader);
	}

#pragma endregion

	// Vertex shader setup
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const char**)&vertSource, NULL);
	glCompileShader(vertexShader);
	if (!VerifyShader(vertexShader))
	{
		// Cleanup
	}

	// Fragment shader setup
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const char**)&fragSource, NULL);
	glCompileShader(fragmentShader);
	if (!VerifyShader(fragmentShader))
	{
		// Cleanup
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
		}
		else
		{
			bValidGeo = true;
		}
	}

	// Tessalation shader setup
	if (geoSource != nullptr)
	{
		geometryShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(tessalationShader, 1, (const char**)&tessSource, NULL);
		glCompileShader(tessalationShader);
		if (!VerifyShader(tessalationShader))
		{
			// Cleanup
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
		CL_CORE_ERROR("Failed linking program.\n", infoLog, "\n");
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

Shader::Shader()
{
}

void Shader::Use()
{
	glUseProgram(m_shaderProgram);
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
		CL_CORE_ERROR("Failed to compile shader.\n", infoLog, "\n");
		delete[] infoLog;


		return false;
	}
	else
	{
		return true;
	}	
}
