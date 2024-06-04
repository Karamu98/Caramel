#pragma once

struct aiNode;
struct aiScene;
class aiMesh;
struct aiMaterialProperty;
struct aiMaterial;

namespace Caramel
{
	class Texture;
	class VertexArray;
	class Material;

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 UV = {0.0f, 0.0f};
	};

	struct Mesh
	{
		Mesh(aiMesh* mesh, const aiScene* scene, const std::string& modelPath);

		std::vector<Vertex> Verticies; // CPU size
		std::vector<uint32_t> Indicies; // CPU size

		std::shared_ptr<VertexArray> Data; // GPU hook
		std::shared_ptr<Material> Material;
	};

	class Model
	{
	public:
		static std::shared_ptr<Model> GetOrLoad(const std::string& path);

		std::unordered_map<std::shared_ptr<Material>, std::vector<Mesh*>> MeshByMaterials;

		const std::string& GetPath() { return m_path; }

		std::vector<Mesh>::iterator begin() { return m_meshes.begin(); }
		std::vector<Mesh>::iterator end() { return m_meshes.end(); }

		std::vector<Mesh>::const_iterator begin() const { return m_meshes.begin(); }
		std::vector<Mesh>::const_iterator end() const { return m_meshes.end(); }

	private:
		static std::shared_ptr<Model> Load(const std::string& path);

		void processNode(aiNode* node, const aiScene* scene);
		void printDebug(aiNode* node, const aiScene* scene);
		void createLUT();

		template<typename T>
		std::string FormatVector(const std::vector<T>& values);

		template<typename T>
		void GetValues(char* data, size_t length, std::vector<T>& outVals);

		std::string GetMaterialInfo(const aiMaterial* material);
		
	private:
		std::string m_path;
		uint32_t m_vertCount;
		std::vector<Mesh> m_meshes;
	};

	template<typename T>
	inline std::string Model::FormatVector(const std::vector<T>& values)
	{
		std::string outVal{};
		int i = 0;
		for (auto val : values)
		{
			if (i > 0)
			{
				outVal += std::format(", {}", val);
			}
			else
			{
				outVal += std::format("{}", val);
			}
			++i;
		}
		return outVal;
	}

	template<typename T>
	inline void Model::GetValues(char* data, size_t length, std::vector<T>& outVals)
	{
		outVals.resize(length / sizeof(T));
		memcpy(&outVals[0], data, length);
	}
}


