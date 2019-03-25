#ifndef __SHADER_H_
#define __SHADER_H_
#include <string>
#include <glm/glm.hpp>
#include <map>



class Shader
{
public :
	/// <summary>
	/// Attempts to create a shader program.
	/// </summary>
	/// <param name='a_vertexPath'>The file path to a valid vertex shader.</param>
	/// <param name='a_fragPath'>The file path to a valid fragment shader.</param>
	/// <param name='a_geometryShader'>The file path to a valid geometry shader. (Optional)</param>
	/// <param name='a_tessalationShader'>The file path to a valid tessalation shader. (Optional)</param>
	Shader(const char* a_vertexPath, const char* a_fragPath, const char* a_geometryShader = nullptr, const char* a_tessalationShader = nullptr);
	~Shader();

	/// <summary>
	/// Binds this shader for use.
	/// </summary>
	void Bind();

	/// <summary>
	/// Unbinds shader.
	/// </summary>
	void Unbind();

	/// <summary>
	/// Sets a boolean uniform value by name.
	/// </summary>
	/// <param name='a_name'>The name of the variable we want to change.</param>
	/// <param name='a_value'>The new value we want the variable to have.</param>
	void SetBool(const std::string& a_name, bool a_value);

	/// <summary>
	/// Sets a integral uniform value by name.
	/// </summary>
	/// <param name='a_name'>The name of the variable we want to change.</param>
	/// <param name='a_value'>The new value we want the variable to have.</param>
	void SetInt(const std::string& a_name, int a_value);

	/// <summary>
	/// Sets a floating point uniform value by name.
	/// </summary>
	/// <param name='a_name'>The name of the variable we want to change.</param>
	/// <param name='a_value'>The new value we want the variable to have.</param>
	void SetFloat(const std::string& a_name, float a_value);

	/// <summary>
	/// Sets a vector4 unifrom value by name.
	/// </summary>
	/// <param name='a_name'>The name of the variable we want to change.</param>
	/// <param name='a_value'>The new value we want the variable to have.</param>
	void SetVec4(const std::string& a_name, glm::vec4 a_value);

	/// <summary>
	/// Sets a vector3 unifrom value by name.
	/// </summary>
	/// <param name='a_name'>The name of the variable we want to change.</param>
	/// <param name='a_value'>The new value we want the variable to have.</param>
	void SetVec3(const std::string& a_name, glm::vec3 a_value);

	/// <summary>
	/// Sets a Matrix4x4 unifrom value by name.
	/// </summary>
	/// <param name='a_name'>The name of the variable we want to change.</param>
	/// <param name='a_value'>The new value we want the variable to have.</param>
	void SetMat4(const std::string& a_name, glm::mat4 a_value);

private:

	/// <summary>
	/// Checks if the shader compiled successfully. Returns false on fail and will print details to console.
	/// </summary>
	/// <param name='a_shaderHandle'>The shader handle we want to test.</param>
	bool VerifyShader(unsigned int& a_shaderHandle);

	unsigned int m_shaderProgram;
};


#endif
