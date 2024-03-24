#pragma once

struct aiNode;
struct aiScene;
class aiMesh;

namespace Caramel
{
	class Texture;
	class VertexArray;

	struct TestVector3
	{
		float X, Y, Z;
	};

	struct TestVector2
	{
		float X, Y;
	};

	struct Vertex
	{
		TestVector3 Position;
		TestVector3 Normal;
		TestVector2 UV = {0.0f, 0.0f};
	};

	struct Mesh
	{
		Mesh(aiMesh* mesh, const aiScene* scene);

		std::vector<Vertex> Verticies;
		std::vector<uint32_t> Indicies;
		std::vector<std::shared_ptr<Texture>> Textures; // Replace with materials

		std::shared_ptr<VertexArray> Data;
	};

	class Model
	{
	public:
		static std::shared_ptr<Model> GetOrLoad(const std::string& path);

		std::vector<Mesh>::iterator begin() { return m_meshes.begin(); }
		std::vector<Mesh>::iterator end() { return m_meshes.end(); }

		std::vector<Mesh>::const_iterator begin() const { return m_meshes.begin(); }
		std::vector<Mesh>::const_iterator end() const { return m_meshes.end(); }

	private:
		static std::shared_ptr<Model> Load(const std::string& path);

		void ProcessNode(aiNode* node, const aiScene* scene);
		void PrintDebug(aiNode* node, const aiScene* scene);
		
	private:
		std::string m_path;
		uint32_t m_vertCount;
		std::vector<Mesh> m_meshes;
	};
}


