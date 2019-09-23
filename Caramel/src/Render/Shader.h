#pragma once

#include <glm/glm.hpp>
#include <unordered_map>

#include "Transform.h"

namespace Caramel
{
	class Shader
	{
	public: // M
		Shader(const std::string& a_sourcePath);
		~Shader();
		bool Bind();
		void Unbind();
		void SetBool(const std::string& a_name, bool a_value, bool a_logErrors = false);
		void SetInt(const std::string& a_name, int a_value, bool a_logErrors = false);
		void SetFloat(const std::string& a_name, float a_value, bool a_logErrors = false);
		void SetVec4(const std::string& a_name, glm::vec4& a_value, bool a_logErrors = false);
		void SetVec3(const std::string& a_name, glm::vec3& a_value, bool a_logErrors = false);
		void SetVec3(const std::string& a_name, glm::detail::_swizzle<3, float, glm::packed_highp, glm::tvec3, 0, 1, 2, -1>& a_value, bool a_logErrors = false);
		void SetVec2(const std::string& a_name, glm::vec2& a_value, bool a_logErrors = false);
		void SetMat4(const std::string& a_name, glm::mat4& a_value, bool a_logErrors = true);
		void SetMat4(const std::string& a_name, Transform& a_value, bool a_logErrors = true);

		inline unsigned int GetProgramID() { return m_shaderProgram; }
		void Recompile();

	private: // M
		bool Compile(const std::unordered_map<unsigned int, std::string>& a_sources);
		unsigned int GetUniformLocation(const std::string& a_name) const;

	private: // V
		unsigned int m_shaderProgram;
		bool m_isValid;
		std::string m_shaderPath;
		mutable std::unordered_map<std::string, unsigned int> m_uniformCache;

	public: // M
		static std::shared_ptr<Shader> CreateShader(const std::string& a_shaderPath);
	private: // M
		static std::string StringFromShaderType(const unsigned int a_type);
		static unsigned int ShaderTypeFromString(const std::string& a_type);
		static std::shared_ptr<std::unordered_map<unsigned int, std::string>> Preprocess(const std::string& a_shaderPath);
	private: // V
		static std::unordered_map<std::string, std::weak_ptr<Shader>> s_shaderLibrary;
	};

}