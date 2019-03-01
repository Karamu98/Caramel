#include "MeshFilter.h"
#include "Log.h"
#include "imgui.h"
#include "Material.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

typedef Component PARENT;

MeshFilter::MeshFilter(Entity * a_pOwner) : PARENT(a_pOwner)
{
	SetComponentType(MESHFILTER);
	m_textBuffer = new char[64];

	m_material = new Material();
}

MeshFilter::~MeshFilter()
{
}

void MeshFilter::OnGUI()
{
	// List Transfrom Component
	ImGui::TextColored(ImVec4(255, 255, 255, 1), "Model Component");
	//ImGui::Text(std::string("Model path: " + m_modelPath).c_str());
	ImGui::InputText("Model Path", m_textBuffer, sizeof(char)*64);
	if (ImGui::Button("Load path"))
	{
		LoadModel();
	}

	m_material->OnGUI();

	ImGui::NewLine();

}

void MeshFilter::LoadModel()
{
	m_modelPath = m_textBuffer;

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(m_modelPath,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices|
		aiProcess_SortByPType);

	if (scene == nullptr)
	{
		CL_CORE_ERROR("Failed to load model at '" + std::string(m_textBuffer) + "'.");
		delete scene;
		return;
	}

	CL_CORE_INFO("Loaded model at'" + std::string(m_textBuffer) + "'.");

	delete[] verts; // Clear whatever we may have had allocated previously
	verts = new Vertex[scene->mMeshes[0]->mNumVertices]; // Assign the space we need for all the verticies

	/// Use these to populate IBO
	//scene->mMeshes[0]->mFaces[0].mIndices;
	//scene->mMeshes[0]->mFaces[0].mNumIndices

#pragma region Conversion
	for (int i = 0; i < scene->mMeshes[0]->mNumVertices; i++)
	{
		verts[i].position.x = scene->mMeshes[0]->mVertices[i].x;
		verts[i].position.y = scene->mMeshes[0]->mVertices[i].y;
		verts[i].position.z = scene->mMeshes[0]->mVertices[i].z;
	}

	if (scene->mMeshes[0]->HasNormals())
	{
		for (int i = 0; i < scene->mMeshes[0]->mNumVertices; i++)
		{
			verts[i].normals.x = scene->mMeshes[0]->mNormals[i].x;
			verts[i].normals.y = scene->mMeshes[0]->mNormals[i].y;
			verts[i].normals.z = scene->mMeshes[0]->mNormals[i].z;
		}
	}
	else
	{
		CL_CORE_WARN("Mesh ", m_textBuffer, " has no normals.");
	}

	if (scene->mMeshes[0]->HasTangentsAndBitangents())
	{
		for (int i = 0; i < scene->mMeshes[0]->mNumVertices; i++)
		{
			verts[i].tans.x = scene->mMeshes[0]->mTangents[i].x;
			verts[i].tans.y = scene->mMeshes[0]->mTangents[i].y;
			verts[i].tans.z = scene->mMeshes[0]->mTangents[i].z;

			verts[i].biTans.x = scene->mMeshes[0]->mBitangents[i].x;
			verts[i].biTans.y = scene->mMeshes[0]->mBitangents[i].y;
			verts[i].biTans.z = scene->mMeshes[0]->mBitangents[i].z;
		}
	}

	//if (scene->mMeshes[0]->HasTextureCoords(0))
	//{
	//	for (int i = 0; i < scene->mMeshes[0]->mNumVertices; i++)
	//	{
	//		verts[i].uvs.x = scene->mMeshes[0]->mTextureCoords[i]->x;
	//		verts[i].uvs.y = scene->mMeshes[0]->mTextureCoords[i]->y;
	//	}
	//}
	#pragma endregion

	//scene->~aiScene(); // Delete the aiScene as we have the data in usable format now

	
}