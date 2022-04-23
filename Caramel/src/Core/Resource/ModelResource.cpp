#include "clpch.h"
#include "Core/Resource/ModelResource.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

#include <glm/ext.hpp>
#include "glad/glad.h"

#include "Core/Utilities.h"

namespace Caramel
{
	ModelResource::ModelResource()
	{
	}

	void ModelResource::Load(const std::string& a_path)
	{
		m_isValid = false;

		// Read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(a_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		// Check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // If is not zero
		{
			CL_CORE_ERROR("Assimp: {0}", importer.GetErrorString());
			return;
		}
		// Retrieve the directory path of the filepath
		m_modelPath = a_path.substr(0, a_path.find_last_of('/'));

		// process ASSIMP's root node recursively
		ProcessAssimpNode(scene->mRootNode, scene);

		m_isValid = true;
	}

	void ModelResource::ProcessAssimpNode(aiNode* a_root, const aiScene* a_scene)
	{
		// process each mesh located at the current node
		for (unsigned int i = 0; i < a_root->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = a_scene->mMeshes[a_root->mMeshes[i]];
			m_meshes.push_back(ProcessAssimpMesh(mesh, a_scene));
		}
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < a_root->mNumChildren; i++)
		{
			ProcessAssimpNode(a_root->mChildren[i], a_scene);
		}
	}

	Mesh ModelResource::ProcessAssimpMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		// Process each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector;

			// Positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.m_position = vector;

			// Normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.m_normal = vector;

			// Texture coordinates
			if (mesh->mTextureCoords[0]) // If the mesh has texCoords
			{
				glm::vec2 vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.m_texCoords = vec;
			}
			else
			{
				vertex.m_texCoords = glm::vec2(0.0f, 0.0f);
			}
				
			
			if (mesh->mTangents > 0)
			{
				// Tangent
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.m_tangent = vector;

				// Bitangent
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.m_bitangent = vector;
			}


			vertices.push_back(vertex);
		}
		// Process each face and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// Retrieve all indices of the face
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// Return a mesh object created from the extracted mesh data
		return Mesh(vertices, indices);
	}

	Mesh::Mesh(std::vector<Vertex>& a_verts, std::vector<unsigned int>& a_inds)
	{
		std::vector<Vertex> vertices = a_verts;
		std::vector<unsigned int> indices = a_inds;

		// Create buffers
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ibo);

		glBindVertexArray(m_vao);

		// Bind and set the vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		// Bind and set the element/indicies buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// Set the vertex attribute pointers
		// Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_normal));
		// Texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_texCoords));
		// Tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_tangent));
		// Bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_bitangent));

		glBindVertexArray(0);

		// Track indicies count
		m_indicesCount = (unsigned int)indices.size();
	}

	Mesh::~Mesh()
	{
		unsigned int buffers[3] = { m_vbo, m_vao, m_ibo };
		glDeleteBuffers(3, buffers);
	}
}