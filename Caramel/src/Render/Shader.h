#pragma once

namespace Caramel
{
	class Shader
	{
	public:
		Shader(const std::string& a_sourcePath);
		~Shader();
		bool Bind();
		void Unbind();
		void SetBool(const std::string& a_name, bool a_value, bool a_logErrors = false);
		void SetInt(const std::string& a_name, int a_value, bool a_logErrors = false);
		void SetFloat(const std::string& a_name, float a_value, bool a_logErrors = false);
		void SetVec4(const std::string& a_name, glm::vec4 a_value, bool a_logErrors = false);
		void SetVec3(const std::string& a_name, glm::vec3 a_value, bool a_logErrors = false);
		void SetVec2(const std::string& a_name, glm::vec2 a_value, bool a_logErrors = false);
		void SetMat4(const std::string& a_name, glm::mat4 a_value, bool a_logErrors = true);

		inline unsigned int GetProgramID() { return m_shaderProgram; }
		void Recompile();

	private:
		unsigned int m_shaderProgram;
		bool m_isValid;
		std::string m_shaderPath;

	private:
		static std::string StringFromShaderType(const unsigned int a_type);
		static unsigned int ShaderTypeFromString(const std::string& a_type);
		static std::shared_ptr<std::unordered_map<unsigned int, std::string>> Preprocess(const std::string& a_shaderPath);
		bool Compile(const std::unordered_map<unsigned int, std::string>& a_sources);
		inline bool LocationValid(unsigned int& a_loc, const std::string& a_name, bool a_logErrors);

	public:
		static std::shared_ptr<Shader> CreateShader(const std::string& a_shaderPath);
	};

}