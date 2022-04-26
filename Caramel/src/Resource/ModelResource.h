#pragma once

#include <glm/glm.hpp>
#include "Render/Shader.h"
#include "Core/Asset.h"

#include "entt/entt.hpp"

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
	class Mesh
	{
	public:
		Mesh(std::vector<Vertex>& a_verts, std::vector<unsigned int>& a_inds);
		~Mesh();

		const unsigned int VertexArrayObject() const { return m_vao; }
		const unsigned int Count() const { return m_indicesCount; }

	private:
		unsigned int m_vao, m_vbo, m_ibo, m_indicesCount;
	};

	class ModelResource : public Asset
	{
	public:
		ModelResource();

		const std::vector<Mesh>& Meshes() { return m_meshes; }

	private:
		void Load(const std::string& a_path);
		void ProcessAssimpNode(aiNode* a_root, const aiScene* a_scene);
		Mesh ProcessAssimpMesh(aiMesh* mesh, const aiScene* scene);

	private:
		std::vector<Mesh> m_meshes;
		std::string m_modelPath;
	};

	class ModelLoader
	{
	};
}
