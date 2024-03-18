#pragma once

#include "Core/RenderAPI/Shader.h"


namespace Caramel
{
	class Shader_OpenGL : public Shader
	{
	public:
		Shader_OpenGL(const ShaderParams& params);
		virtual ~Shader_OpenGL();

		// Inherited via Shader
		void Bind() override;
		void Unbind() override;

		virtual void SetValue(const std::string& name, ShaderDataType type, void* value) override;


	private:
		uint32_t m_id;
		std::unordered_map<std::string, uint32_t> m_uniformLocations;
	};
}


