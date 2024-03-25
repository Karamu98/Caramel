#include "clpch.h"
#include "Material.h"
#include <Core/RenderAPI/Shader.h>
#include <Core/RenderAPI/Texture.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <filesystem>

namespace Caramel
{
	std::unordered_map<std::string, std::shared_ptr<Material>> Material::s_Loaded;

	static std::unordered_map<std::string, MaterialProperty> AssimpToCaramelLUT =
	{
		{"$clr.diffuse", {"u_DiffuseColour", ShaderDataType::Float3}},
		{"$clr.specular", {"u_SpecularColour", ShaderDataType::Float3}},
		{"$clr.ambient", {"u_AmbientColour", ShaderDataType::Float3}}
	};

	inline std::string SemanticIndexToUniformName(uint32_t semantic, uint32_t idx)
	{
		switch (semantic)
		{
		case aiTextureType_DIFFUSE: return "u_DiffuseMap" + std::to_string(idx);
		case aiTextureType_SPECULAR: return "u_SpecularMap" + std::to_string(idx);
		case aiTextureType_AMBIENT: return "u_AmbientMap" + std::to_string(idx);
		case aiTextureType_EMISSIVE: return "u_EmissiveMap" + std::to_string(idx);
		case aiTextureType_HEIGHT: return "u_HeightMap" + std::to_string(idx);
		case aiTextureType_NORMALS: return "u_NormalsMap" + std::to_string(idx);
		case aiTextureType_SHININESS: return "u_ShininessMap" + std::to_string(idx);
		case aiTextureType_OPACITY: return "u_OpacityMap" + std::to_string(idx);
		case aiTextureType_DISPLACEMENT: return "u_DisplacementMap" + std::to_string(idx);
		case aiTextureType_LIGHTMAP: return "u_LightMap" + std::to_string(idx);
		case aiTextureType_REFLECTION: return "u_ReflectionMap" + std::to_string(idx);
		case aiTextureType_BASE_COLOR: return "u_Base_ColorMap" + std::to_string(idx);
		case aiTextureType_NORMAL_CAMERA: return "u_Normal_CameraMap" + std::to_string(idx);
		case aiTextureType_EMISSION_COLOR: return "u_Emission_ColorMap" + std::to_string(idx);
		case aiTextureType_METALNESS: return "u_MetalnessMap" + std::to_string(idx);
		case aiTextureType_DIFFUSE_ROUGHNESS: return "u_Diffuse_RoughnessMap" + std::to_string(idx);
		case aiTextureType_AMBIENT_OCCLUSION: return "u_Ambient_OcclusionMap" + std::to_string(idx);
		case aiTextureType_SHEEN: return "u_SheenMap" + std::to_string(idx);
		case aiTextureType_CLEARCOAT: return "u_ClearcoatMap" + std::to_string(idx);
		case aiTextureType_TRANSMISSION: return "u_TransmissionMap" + std::to_string(idx);
		default:
		case aiTextureType_UNKNOWN: return "Unknown";
		}
	}

	static TextureProperties REMOVEME_Default
	{
		TextureType::T_2D,
		MinFilterMode::Linear,
		MagFilterMode::Linear,
		TextureWrapMode::Repeat,
		TextureWrapMode::Repeat,
		TextureWrapMode::Repeat,
	};

	void Material::Bind()
	{
		if (Shader == nullptr)
		{
			return;
		}

		Shader->Bind();
		for (const auto& prop : Properties)
		{
			Shader->SetValue(prop.UniformName, prop.Type, (void*)prop.Data.data());
		}

		for (int i = 0; i < Textures.size(); ++i)
		{
			Textures[i].Texture->Bind(i);
			Shader->SetTexture(Textures[i].UniformName, i);
		}
	}

	void Material::CheckForDeadProperties()
	{
		if (!Shader)
		{
			CL_CORE_ERROR("Material: ({}) - Tried to check for dead properties but found no shader", m_name);
			return;
		}

		for (int i = Properties.size() - 1; i >= 0; --i)
		{
			if (!Shader->HasUniform(Properties[i].UniformName))
			{
				CL_CORE_WARN("Material uniform ({0}) was not found in shader, removing...", Properties[i].UniformName);
				Properties.erase(Properties.begin() + i);
			}
		}
	}

	std::shared_ptr<Material> Material::Get(const std::string& materialName)
	{
		if (s_Loaded.find(materialName) != s_Loaded.end())
		{
			return s_Loaded[materialName];
		}

		return nullptr;
	}

	std::shared_ptr<Material> Material::Create(const std::string& materialName)
	{
		if (s_Loaded.find(materialName) != s_Loaded.end())
		{
			// Return null as control is expected to initialise this material afterwards
			CL_CORE_ERROR("Material with the name {} already exists");
			return nullptr;
		}

		s_Loaded[materialName] = std::make_shared<Material>();
		s_Loaded[materialName]->m_name = materialName;

		return s_Loaded[materialName];
	}

	std::shared_ptr<Material> Material::CreateFromAssimp(const std::string& modelPath, const std::string& materialName, const aiMaterial* aiMat)
	{
		if (s_Loaded.find(materialName) != s_Loaded.end())
		{
			// Return null as control is expected to initialise this material afterwards
			CL_CORE_ERROR("Material with the name {} already exists");
			return nullptr;
		}

		s_Loaded[materialName] = std::make_shared<Material>();
		s_Loaded[materialName]->m_name = materialName;

		std::shared_ptr<Material> newMaterial = s_Loaded[materialName];

		newMaterial->Properties.reserve(aiMat->mNumProperties);

		for (int i = 0; i < aiMat->mNumProperties; ++i)
		{
			aiMaterialProperty* currentProp = aiMat->mProperties[i];
			std::string curPropName = currentProp->mKey.C_Str();
			if (AssimpToCaramelLUT.find(curPropName) != AssimpToCaramelLUT.end())
			{
				auto& newPropNeedsData = newMaterial->Properties.emplace_back(AssimpToCaramelLUT[curPropName]);
				newPropNeedsData.Data.resize(currentProp->mDataLength);
				memcpy(&newPropNeedsData.Data[0], currentProp->mData, currentProp->mDataLength);
			}
			else if(curPropName == "$tex.file")
			{
				std::vector<char> values(currentProp->mDataLength - 5);
				memcpy(&values[0], &currentProp->mData[4], currentProp->mDataLength - 5);
				std::string texPath(values.begin(), values.end());

				std::filesystem::path filePath(modelPath);
				texPath = filePath.parent_path().string() + "/" + texPath;

				auto newTex = Texture::Create({ texPath, REMOVEME_Default });

				newMaterial->Textures.push_back({ SemanticIndexToUniformName(currentProp->mSemantic, currentProp->mIndex) , newTex});
			}
			else if (curPropName == "$mat.shadingm")
			{
				uint32_t mode = *(uint32_t*)currentProp->mData;
				newMaterial->Shader = Shader::CreateDefault((ShadingModel)mode);
			}
			else
			{
				//CL_CORE_ERROR("Assimp property {} not handled currently", curPropName);
			}
		}

		newMaterial->CheckForDeadProperties();


		return newMaterial;
	}
}