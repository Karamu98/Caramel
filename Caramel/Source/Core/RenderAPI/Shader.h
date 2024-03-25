#pragma once

#include "Core/RenderAPI/ShaderDataType.h"

namespace Caramel
{
	enum class ShadingModel
	{
		Flat = 1,
		Gouraud,
		Phong,
		Blinn,
		Toon,
		OrenNayar,
		Minnaert,
		CookTorrance,
		NoShading,
		Fresnel,
		PBR_BRDF,
	};
	

	struct ShaderParams
	{
		const std::string& vertexSrc;
		const std::string& fragmentSrc;
		const std::vector<std::string>& Uniforms = {};
	};

	class Shader
	{
	public:
		virtual ~Shader() {};

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetValue(const std::string& name, ShaderDataType type, void* value) = 0;
		virtual void SetTexture(const std::string& name, uint32_t unit) = 0;
		virtual bool HasUniform(const std::string& name, bool checkClient = false) = 0;

		static std::shared_ptr<Shader> Create(const ShaderParams& params);
		static std::shared_ptr<Shader> CreateFromFiles(const std::string& vertexPath, const std::string& fragPath, const std::vector<std::string>& uniforms = {});
		static std::shared_ptr<Shader> CreateDefault(ShadingModel model);

	private:
		static bool LoadShaderCode(const std::string& path, std::string& outCode);
	};
}


