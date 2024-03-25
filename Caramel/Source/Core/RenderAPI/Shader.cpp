#include "clpch.h"
#include "Shader.h"

#include <fstream>

#include "Core/Application.h"

#include "Platform/RenderAPI/OpenGL/Shader_OpenGL.h"

namespace Caramel
{
	struct DefaultShaderConfig
	{
		const std::string VertexPath;
		const std::string FragmentPath;
		const std::vector<std::string> Uniforms;
	};

	static std::unordered_map<ShadingModel, DefaultShaderConfig> S_DefaultShaders =
	{
		{ 
			ShadingModel::Phong, 
			{
				"../Caramel/Source/Core/Renderer/Shaders/Phong.vert", "../Caramel/Source/Core/Renderer/Shaders/Phong.frag", 
				{
					"u_View", "u_Projection", "u_ModelTransform",

					"u_ViewPos", "u_LightPos", "u_LightColour", 
					"u_DiffuseColour", "u_AmbientColour", "u_SpecularColour",
					"u_DiffuseMap0", "u_SpecularMap0"
				}
			}
		},
		{
			ShadingModel::NoShading,
			{
				"../Caramel/Source/Core/Renderer/Shaders/Unlit.vert", "../Caramel/Source/Core/Renderer/Shaders/Unlit.frag",
				{
					"u_View", "u_Projection", "u_ModelTransform",

					"u_DiffuseMap0", "u_DiffuseColour",
				}
			}
		}
	};

	std::shared_ptr<Shader> Caramel::Shader::Create(const ShaderParams& params)
	{
		switch (Application::GetRenderAPI())
		{
		case WindowRenderAPI::OpenGL: return std::make_shared<Shader_OpenGL>(params);
		}

		CL_CORE_ASSERT(false);
		return nullptr;
	}

	std::shared_ptr<Shader> Shader::CreateFromFiles(const std::string& vertexPath, const std::string& fragPath, const std::vector<std::string>& uniforms)
	{
		std::string vSrc{};
		std::string fSrc{};

		if (!LoadShaderCode(vertexPath, vSrc) || !LoadShaderCode(fragPath, fSrc))
		{
			CL_CORE_ERROR("Failed to load shader source");
			return nullptr;
		}

		return Create({ vSrc, fSrc, uniforms });
	}

	std::shared_ptr<Shader> Shader::CreateDefault(ShadingModel model)
	{
		if (S_DefaultShaders.find(model) == S_DefaultShaders.end())
		{
			CL_CORE_FATAL("This shader model type isn't implemented ({}). Falling back on unlit...", magic_enum::enum_name(model));
			DefaultShaderConfig conf = S_DefaultShaders[ShadingModel::NoShading];
			return CreateFromFiles(conf.VertexPath, conf.FragmentPath, conf.Uniforms);
		}

		DefaultShaderConfig conf = S_DefaultShaders[model];
		return CreateFromFiles(conf.VertexPath, conf.FragmentPath, conf.Uniforms);
	}

	bool Shader::LoadShaderCode(const std::string& path, std::string& outCode)
	{
		std::ifstream shaderFile;
		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try 
		{
			shaderFile.open(path);
			std::stringstream shaderStream;
			shaderStream << shaderFile.rdbuf();
			shaderFile.close();
			outCode = shaderStream.str();
			return true;
		}
		catch (std::ifstream::failure& e) 
		{
			CL_CORE_ERROR("Failed to find shader code at path {0}. Reason : {1}", path, e.what());
		}
		return false;
	}
}

