#pragma once
#include "Core/RenderAPI/ShaderDataType.h"

struct aiMaterial;

namespace Caramel
{
    class Shader;
    class Texture;

    struct MaterialProperty
    {
        std::string UniformName;
        ShaderDataType Type;
        std::vector<char> Data;
    };

    struct MaterialTextures
    {
        std::string UniformName;
        std::shared_ptr<Texture> Texture;
    };

	class Material
	{
	public:
        void Bind();

        std::vector<MaterialTextures> Textures;
        std::vector<MaterialProperty> Properties;
        std::shared_ptr<Shader> Shader;
        

        void CheckForDeadProperties();

        static std::shared_ptr<Material> Get(const std::string& materialName);
        static std::shared_ptr<Material> Create(const std::string& materialName);
        static std::shared_ptr<Material> CreateFromAssimp(const std::string& modelPath, const std::string& materialName, const aiMaterial* aiMat); // Probably shouldnt live here 

    private:
        std::string m_name;

        static std::unordered_map<std::string, std::shared_ptr<Material>> s_Loaded;
	};
}


