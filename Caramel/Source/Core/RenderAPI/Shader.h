#pragma once

namespace Caramel
{
	struct ShaderParams
	{
		const std::string& vertexSrc;
		const std::string& fragmentSrc;
	};

	class Shader
	{
	public:
		virtual ~Shader() {};

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		static std::shared_ptr<Shader> Create(const ShaderParams& params);
	};
}


