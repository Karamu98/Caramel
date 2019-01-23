#include "MeshFilter.h"
#include "Log.h"
#include "imgui.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

typedef Component PARENT;

MeshFilter::MeshFilter(Entity * a_pOwner) : PARENT(a_pOwner)
{
	SetComponentType(MESHFILTER);
	m_textBuffer = new char[64];
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
		LoadModel(std::string(m_textBuffer));
	}
	ImGui::NewLine();
}

void MeshFilter::LoadModel(std::string a_path)
{
	m_modelPath = a_path.c_str();

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(a_path,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices|
		aiProcess_SortByPType);

	if (scene == nullptr)
	{
		CL_CORE_ERROR(std::string("Failed to load model '" + a_path + "'."));
		delete scene;
		return;
	}

	CL_CORE_INFO(std::string("Loaded model '" + a_path + "'!"));

	//scene->mMeshes[0]->mVertices //The meshes verticies
}

void MeshFilter::Draw(unsigned int a_uProgramID, unsigned int a_uVBO, unsigned int a_uIBO)
{
}
