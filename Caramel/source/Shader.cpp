#include "Shader.h"
#include "Utilities.h"
#include "Log.h"
#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Renderer.h"


// If this is defined, we don't want to delete shaders to allow for easy debugging
#ifndef CL_DEBUG

#define DeleteShader(...) glDeleteShader(__VA_ARGS__)

#else

#define DeleteShader

#endif

unsigned char* FileToBuffer(const char* a_sPath)
{
	// open file for text reading
	FILE* pFile = fopen(a_sPath, "rb");
	if (pFile == nullptr)
	{
		return nullptr;
	}

	// get number of bytes in file
	fseek(pFile, 0, SEEK_END);
	unsigned int uiLength = (unsigned int)ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	// allocate buffer and read file contents
	unsigned char* acBuffer = new unsigned char[uiLength + 1];
	memset(acBuffer, 0, uiLength + 1);
	fread(acBuffer, sizeof(unsigned char), uiLength, pFile);

	fclose(pFile);
	return acBuffer;
}



Shader::Shader(const char * a_vertexPath, const char * a_fragPath, const char * a_geometryPath, const char * a_tessCtrlPath, const char* a_tessEvalPath)
{
	// These will hold the source of our files if we specify them
	unsigned char* vertSource;
	unsigned char* fragSource;
	unsigned char* geoSource = nullptr;
	unsigned char* tessCtrlSrc = nullptr;
	unsigned char* tessEvalSrc = nullptr;

	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int geometryShader = 0;
	unsigned int tessCtrlShader = 0;
	unsigned int tessEvalShader = 0;

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
		vertSource = FileToBuffer(a_vertexPath);
		if (vertSource == nullptr)
		{
			CL_CORE_ERROR("A vertex shader could not be loaded at " + std::string(a_vertexPath));
			return;
		}
		else
		{
			CL_CORE_INFO("Vertex shader loaded at " + std::string(a_vertexPath));
		}
	}

	if (a_fragPath == nullptr)
	{
		CL_CORE_ERROR("A fragment shader must be supplied, cannot be nullptr.");
		return;
	}
	else
	{
		fragSource = FileToBuffer(a_fragPath);
		if (fragSource == nullptr)
		{
			CL_CORE_ERROR("A fragment shader could not be loaded at " + std::string(a_fragPath));
			return;
		}
		else
		{
			CL_CORE_INFO("Fragment shader loaded at " + std::string(a_fragPath));
		}
	}

	if (a_geometryPath != nullptr)
	{
		geoSource = FileToBuffer(a_geometryPath);
		if (geoSource == nullptr)
		{
			CL_CORE_ERROR("A geometry shader could not be loaded at ", std::string(a_geometryPath));
			return;
		}
		else
		{
			CL_CORE_INFO("Geometry shader loaded at " + std::string(a_geometryPath));
		}
	}

	if (a_tessCtrlPath != nullptr)
	{
		tessCtrlSrc = FileToBuffer(a_tessCtrlPath);
		if (tessCtrlSrc == nullptr)
		{
			CL_CORE_ERROR("A tessalation control shader could not be loaded at " + std::string(a_tessCtrlPath));
			return;
		}
		else
		{
			CL_CORE_INFO("Tessalation control shader loaded at " + std::string(a_tessCtrlPath));
		}
	}

	if (a_tessEvalPath != nullptr)
	{
		tessEvalSrc = FileToBuffer(a_tessEvalPath);
		if (tessEvalSrc == nullptr)
		{
			CL_CORE_ERROR("A tessalation evaluation shader could not be loaded at " + std::string(a_tessEvalPath));
			return;
		}
		else
		{
			CL_CORE_INFO("Tessalation evaluation shader loaded at " + std::string(a_tessEvalPath));
		}
	}

#pragma endregion

	// Vertex shader setup
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const char**)&vertSource, NULL);
	glCompileShader(vertexShader);
	if (!VerifyShader(vertexShader, a_vertexPath))
	{
		// Cleanup
		return;
	}

	// Fragment shader setup
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const char**)&fragSource, NULL);
	glCompileShader(fragmentShader);
	if (!VerifyShader(fragmentShader, a_fragPath))
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
		if (!VerifyShader(geometryShader, a_geometryPath))
		{
			// Cleanup
			return;
		}
		else
		{
			bValidGeo = true;
		}
	}

	// Tessalation control shader setup
	if (tessCtrlSrc != nullptr)
	{
		tessCtrlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(tessCtrlShader, 1, (const char**)&tessCtrlSrc, NULL);
		glCompileShader(tessCtrlShader);
		if (!VerifyShader(tessCtrlShader, a_tessCtrlPath))
		{
			// Cleanup
			return;
		}

		// Tessalation evaluation shader setup
		if (tessEvalSrc != nullptr)
		{
			tessEvalShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
			glShaderSource(tessEvalShader, 1, (const char**)& tessEvalSrc, NULL);
			glCompileShader(tessEvalShader);
			if (!VerifyShader(tessEvalShader, a_tessEvalPath))
			{
				// Cleanup
				return;
			}
			else
			{
				bValidTess = true;
			}
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
		glAttachShader(m_shaderProgram, tessCtrlShader);
		glAttachShader(m_shaderProgram, tessEvalShader);
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
		DeleteShader(tessCtrlShader);
		DeleteShader(tessEvalShader);
	}

	CL_GETGL_ERRORS;


	delete vertSource;
	delete fragSource;
	delete geoSource;
	delete tessCtrlSrc;
	delete tessEvalSrc;

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

bool Shader::VerifyShader(unsigned int & a_shaderHandle, std::string a_path)
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

		CL_CORE_ERROR("Failed to compile shader at " + std::string(a_path) + ".\n" + log);
		delete[] infoLog;


		return false;
	}
	else
	{
		return true;
	}	
}
