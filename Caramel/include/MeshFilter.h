#ifndef _MESH_FILTER_H__
#define _MESH_FILTER_H__

//includes
#include "Component.h"
#include <string>
#include <glm/glm.hpp>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Mesh.h"
#include "Texture.h"

class Entity;
class Shader;
class Camera;

class MeshFilter : public Component
{
public:

	MeshFilter(Entity* a_pOwner);
	~MeshFilter();

	virtual void Update(float a_fDeltaTime) {};
	virtual void OnGUI();

	void LoadModel();
	void LoadModel(std::string a_path);
	void SetShader(Shader* a_shaderToUse);
	void Draw(Camera* a_camera);


private:

	// Model loading
	void ProcessNode(aiNode* a_node, const aiScene* a_scene);
	Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

	// Model load data
	std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	std::vector<Mesh> meshes;
	const char* m_modelPath;

	char* m_textBuffer; // For ImGui

	Shader* m_modelShader;

};

#endif
