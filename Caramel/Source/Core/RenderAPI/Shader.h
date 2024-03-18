#pragma once

#include "Core/RenderAPI/ShaderDataType.h"

namespace Caramel
{
	struct ShaderParams
	{
		const std::string& vertexSrc;
		const std::string& fragmentSrc;
		const std::vector<std::string>& Uniforms;
	};

	class Shader
	{
	public:
		virtual ~Shader() {};

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetValue(const std::string& name, ShaderDataType type, void* value) = 0;

		static std::shared_ptr<Shader> Create(const ShaderParams& params);
	};
}


