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

/// Ian Hudson helped a lot with helping me understand the model loading and data management.

enum MeshType
{
	SOLID,
	TRANSPARENTS,
	ANIMATINGSOLID,
	ANIMATINGTRANSPARENT
};

class MeshFilter : public Component
{
public:

	MeshFilter(Entity* a_pOwner, MeshType a_type);
	~MeshFilter();

	virtual void Update(float a_fDeltaTime) {};
	virtual void OnGUI();
	virtual bool OnDelete();

	void LoadModel();
	void LoadModel(std::string a_path);
	void Draw(Shader* a_shader);

	MeshType GetType();

private:

	// Model loading
	void ProcessNode(aiNode* a_node, const aiScene* a_scene);
	Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

	void UnloadModel();

	// Model load data
	std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	std::vector<Mesh> meshes;

	std::string m_dir;

	MeshType m_meshType;


	char m_modelTextbuff[128]; // For ImGui
	char m_shaderTextbuff[128]; // For ImGui
};

#endif
