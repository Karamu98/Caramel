#include "clpch.h"
#include "Render/Model.h"

#include <glm/ext.hpp>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

#include "glad/glad.h"

#include "Core/Log.h"
#include "Render/Camera.h"
#include "Core/Utilities.h"

namespace Caramel
{
	std::unordered_map<std::string, std::weak_ptr<Model>> Model::s_modelLibrary;

	std::shared_ptr<Model> Model::LoadModel(const std::string& a_path)
	{
		std::shared_ptr<Model> newModel;
		// If the model is tracked
		if (s_modelLibrary.find(a_path) != s_modelLibrary.end())
		{
			// And if the model is still loaded, return the model
			if (newModel = s_modelLibrary[a_path].lock())
			{
				return newModel;
			}
		}

		newModel = std::make_shared<Model>();

		if (newModel->Load(a_path))
		{
			// Register new model 
			s_modelLibrary[a_path] = newModel;
			return newModel;
		}
		else
		{
			return nullptr;
		}
	}

	Model::Model() : m_isValid(false), m_hasPreview(false)
	{
	}

	void Model::Draw()
	{
		if (m_isValid)
		{
			for (Mesh mesh : m_meshes)
			{
				mesh.Draw();
			}
		}
	}

	void Model::Reload(const std::string& a_newPath)
	{
		m_isValid = false;
		if (m_hasPreview)
		{
			glDeleteFramebuffers(1, &m_previewBuffer);
			glDeleteTextures(1, &m_previewTexture);
			m_hasPreview = false;
		}
		m_modelPath = a_newPath;
		m_meshes.clear();

		Load(a_newPath);
	}

	// TODO: This is garbage
	unsigned int Model::GetPreviewTex()
	{
		if (m_hasPreview)
		{
			return m_previewTexture;
		}
		// Grab previously bound
		int previousBound;
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &previousBound);

		int previousViewPort[4];
		glGetIntegerv(GL_VIEWPORT, &previousViewPort[0]);

		// Generate a framebuffer
		glGenFramebuffers(1, &m_previewBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_previewBuffer);

		// Create a color attachment texture
		glGenTextures(1, &m_previewTexture);
		glBindTexture(GL_TEXTURE_2D, m_previewTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 80, 80, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_previewTexture, 0);

		// Set the list of draw buffers.
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			CL_CORE_ERROR("Framebuffer not complete.");
		}
		glViewport(0, 0, 80, 80);

		std::shared_ptr<Shader> previewShader = Shader::CreateShader(Utility::GetWorkingDir() + "resources/engine/shaders/modelPreview.glsl");		
		if (previewShader->Bind())
		{
			Transform modelTrans;
			modelTrans.SetPosition(glm::vec3(0.0f));

			glm::mat4x4 camProjMat = glm::ortho(m_bounds.GetMinBound().x, m_bounds.GetMaxBound().x, m_bounds.GetMinBound().z, m_bounds.GetMaxBound().z);
			glm::mat4x4 camViewMat = glm::inverse(glm::lookAt(glm::vec3(5), modelTrans.GetPos(), glm::vec3(0, 1, 0)));

			previewShader->SetVec3("previewColour", glm::vec3(1.0f, 1.0f, 1.0f));
			previewShader->SetMat4("objMatrix", modelTrans, true);
			previewShader->SetInt("gTex", 0);
			previewShader->SetMat4("camProjView", camProjMat * camViewMat);
			Draw();
			m_hasPreview = true;
		}
		else
		{
			CL_CORE_ERROR("Unable to bind model preview shader.");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, previousBound);
		glViewport(previousViewPort[0], previousViewPort[1], previousViewPort[2], previousViewPort[3]);
		Utility::GetGLErrors();
		return m_previewTexture;
	}

	bool Model::Load(const std::string& a_path)
	{
		// If we're already valid, delete
		if (m_isValid)
		{
			m_meshes.clear();
		}

		m_isValid = false;

		// Read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(a_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		// Check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // If is not zero
		{
			CL_CORE_ERROR("Assimp: {0}", importer.GetErrorString());
			return false;
		}
		// Retrieve the directory path of the filepath
		m_modelPath = a_path.substr(0, a_path.find_last_of('/'));

		m_minmax = new glm::vec3[2];

		// process ASSIMP's root node recursively
		ProcessAssimpNode(scene->mRootNode, scene);

		m_bounds.SetBounds(m_minmax[0], m_minmax[1]);
		delete m_minmax;

		m_isValid = true;
		return true;
	}

	void Model::ProcessAssimpNode(aiNode* a_root, const aiScene* a_scene)
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

	Mesh Model::ProcessAssimpMesh(aiMesh* mesh, const aiScene* scene)
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
			TestMinMax(vector);

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

	void Model::TestMinMax(const glm::vec3& a_point)
	{
		if (a_point.x < m_minmax[0].x)
		{
			m_minmax[0].x = a_point.x;
		}
		if (a_point.y < m_minmax[0].y)
		{
			m_minmax[0].y = a_point.y;
		}
		if (a_point.z < m_minmax[0].z)
		{
			m_minmax[0].z = a_point.z;
		}

		if (a_point.x > m_minmax[1].x)
		{
			m_minmax[1].x = a_point.x;
		}
		if (a_point.y > m_minmax[1].y)
		{
			m_minmax[1].y = a_point.y;
		}
		if (a_point.z > m_minmax[1].z)
		{
			m_minmax[1].z = a_point.z;
		}
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

	void Mesh::Draw()
	{
		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}