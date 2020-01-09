#pragma once

#include <glm/glm.hpp>
#include "Render/Shader.h"
#include "Core/BoundingCube.h"

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
		void Reload(const std::string& a_newPath);
		unsigned int GetPreviewTex();

	private:
		bool Load(const std::string& a_path);
		void ProcessAssimpNode(aiNode* a_root, const aiScene* a_scene);
		Mesh ProcessAssimpMesh(aiMesh* mesh, const aiScene* scene);
		void TestMinMax(const glm::vec3& a_point);

	private:
		std::vector<Mesh> m_meshes;
		std::string m_modelPath;
		bool m_isValid;
		unsigned int m_previewTexture;
		unsigned int m_previewBuffer;
		bool m_hasPreview;

		BoundingCube m_bounds;

		// Cache
		glm::vec3* m_minmax;
	};
}
