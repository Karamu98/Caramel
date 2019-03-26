#include "MeshFilter.h"
#include "Log.h"
#include "imgui.h"
#include "gl_core_4_4.h"
#include "Shader.h"
#include "Entity.h"


typedef Component PARENT;

MeshFilter::MeshFilter(Entity * a_pOwner) : PARENT(a_pOwner)
{
}

MeshFilter::~MeshFilter()
{	
}

void MeshFilter::OnGUI()
{
	// List Transfrom Component
	ImGui::TextColored(ImVec4(255, 255, 255, 1), "Model Component");
	ImGui::InputText("Model Path", m_modelTextbuff, IM_ARRAYSIZE(m_modelTextbuff));
	if (ImGui::Button("Load path"))
	{
		LoadModel();
	}
	ImGui::InputText("Shader name", m_shaderTextbuff, IM_ARRAYSIZE(m_shaderTextbuff));
	if (ImGui::Button("Attach to Shader"))
	{
		
	}


	ImGui::NewLine();

}

bool MeshFilter::OnDelete()
{
	ImGui::PushID(GetOwnerEntity() + GetModelNumber());
	if (ImGui::Button("Mesh"))
	{
		m_owningShader->UnregisterRenderable(this);
		GetOwnerEntity()->DeleteComponent(this);
		ImGui::PopID();
		return true;
	}
	else
	{
		ImGui::PopID();
		return false;
	}
}

void MeshFilter::LoadModel()
{
	UnloadModel();

	std::string location = m_modelTextbuff;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(location, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (scene == nullptr)
	{
		CL_CORE_ERROR("Failed to load model at '" + std::string(m_modelTextbuff) + "'.");
		delete scene;
		return;
	}

	m_dir = std::string(m_modelTextbuff).substr(0, std::string(m_modelTextbuff).find_last_of('/'));

	CL_CORE_INFO("Loaded model at'" + std::string(m_modelTextbuff) + "'.");

	// process ASSIMP's root node recursively
	ProcessNode(scene->mRootNode, scene);
}

void MeshFilter::LoadModel(std::string a_path)
{
	UnloadModel();

	memcpy(m_modelTextbuff, a_path.c_str(), sizeof(char) * 127);

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(a_path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (scene == nullptr)
	{
		CL_CORE_ERROR("Failed to load model at '" + std::string(m_modelTextbuff) + "'.");
		delete scene;
		return;
	}

	m_dir = std::string(a_path).substr(0, std::string(a_path).find_last_of('/'));

	CL_CORE_INFO("Loaded model at'" + a_path + "'.");

	// process ASSIMP's root node recursively
	ProcessNode(scene->mRootNode, scene);
}

void MeshFilter::Draw(Shader* a_shader)
{
	glm::mat4 m4ModelMat = *GetOwnerEntity()->GetTransform()->GetMatrix();
	a_shader->SetMat4("Model", m4ModelMat); //:::CONTINUE::: You need a way to manipulate all the meshes in a model when it moves, you also need to get world space for any of these meshes
	a_shader->SetMat4("NormalMatrix", glm::transpose(glm::inverse(m4ModelMat)));

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(a_shader);
	}

}

void MeshFilter::ProcessNode(aiNode * a_node, const aiScene * a_scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < a_node->mNumMeshes; i++)
	{
		aiMesh *mesh = a_scene->mMeshes[a_node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, a_scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < a_node->mNumChildren; i++)
	{
		ProcessNode(a_node->mChildren[i], a_scene);
	}
}

Mesh MeshFilter::ProcessMesh(aiMesh *mesh, const aiScene *scene)
{
	// data to fill
	std::vector<Vertex> vertices;
	vertices.reserve(mesh->mNumVertices);
	std::vector<unsigned int> indices;
	indices.reserve(mesh->mNumFaces * mesh->mFaces[0].mNumIndices);
	std::vector<Texture> textures;
	textures.reserve(4);

	// Walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec4 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		vector.w = 0;
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;
		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normals = vector;
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.uvs = vec;
		}
		else
			vertex.uvs = glm::vec2(0.0f, 0.0f);
		// tangent
		if (mesh->mTangents != nullptr)
		{
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.tans = vector;
			// bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.biTans = vector;
			vertices.push_back(vertex);
		}
		else
		{
			vector.x = 0;
			vector.y = 0;
			vector.z = 0;
			vertex.tans = vector;
			// bitangent
			vector.x =  0;
			vector.y =  0;
			vector.z =  0;
			vertex.biTans = vector;
			vertices.push_back(vertex);
		}
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	// 1. diffuse maps
	std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");

	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	// return a mesh object created from the extracted mesh data
	return Mesh(vertices, indices, textures);
}

std::vector<Texture> MeshFilter::LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString filePath;
		mat->GetTexture(type, i, &filePath);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].m_filePath, filePath.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.m_textureID = texture.TextureFromFile(filePath.C_Str(), m_dir, false);
			texture.m_type = typeName;
			texture.m_filePath = filePath.C_Str();
			textures.push_back(texture);
			//textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

void MeshFilter::UnloadModel()
{
	if (meshes.size() > 0)
	{
		for (int i = 0; i < meshes.size(); i++)
		{
			meshes[i].Unload();
		}
	}

	if (textures_loaded.size() > 0)
	{
		for (Texture tex : textures_loaded)
		{
			tex.Unload();
		}
	}

	textures_loaded.clear();
	meshes.clear();
	m_dir = std::string();
	
}
