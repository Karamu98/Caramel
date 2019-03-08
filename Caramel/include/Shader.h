#ifndef __SHADER_H_
#define __SHADER_H_
#include <string>


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
	
	Shader();

	/// <summary>
	/// Binds this shader for use.
	/// </summary>
	void Use();

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


private:

	/// <summary>
	/// Checks if the shader compiled successfully. Returns false on fail and will print details to console.
	/// </summary>
	/// <param name='a_shaderHandle'>The shader handle we want to test.</param>
	bool VerifyShader(unsigned int& a_shaderHandle);

	unsigned int m_shaderProgram;
};


#endif