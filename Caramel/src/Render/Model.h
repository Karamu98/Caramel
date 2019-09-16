#pragma once

#include <glm/glm.hpp>
#include "Render/Shader.h"

struct Vertex
{
	glm::vec3 m_position;
	glm::vec3 m_normal;
	glm::vec2 m_texCoords;
	glm::vec3 m_tangent;
	glm::vec3 m_bitangent;
};

struct aiNode;
struct aiScene;
struct aiMesh;

namespace Caramel
{
	// TODO: Mesh batching
	class Mesh
	{
	public:
		Mesh(std::vector<Vertex>& a_verts, std::vector<unsigned int>& a_inds);
		~Mesh();

		void Draw();

	private:
		unsigned int m_vao, m_vbo, m_ibo, m_indicesCount;
	};

	class Model
	{
	public:
		static std::shared_ptr<Model> LoadModel(const std::string& a_path);
	private:
		static std::unordered_map<std::string, std::weak_ptr<Model>> s_modelLibrary;



	public:
		Model();

		void Draw();

	private:
		bool Load(const std::string& a_path);
		void ProcessAssimpNode(aiNode* a_root, const aiScene* a_scene);
		Mesh ProcessAssimpMesh(aiMesh* mesh, const aiScene* scene);

	private:
		std::vector<Mesh> m_meshes;
		std::string m_modelPath;
		bool m_isValid;
	};
}
