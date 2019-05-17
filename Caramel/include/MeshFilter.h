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
#include <fstream>
#include "Texture.h"

class Entity;
class Shader;

/// Ian Hudson helped a lot with helping me understand the model loading and data management.

enum MeshType : unsigned int
{
	SOLID = 1, // First bit
	TRANSPARENTS = 2,
	ANIMATING = 4,
	SHADCAST = 16 // 4th bit etc
};

inline MeshType operator|(MeshType a_1, MeshType a_2) { return static_cast<MeshType>(static_cast<int>(a_1) | static_cast<int>(a_2)); }
inline MeshType operator&(MeshType a_1, MeshType a_2) { return static_cast<MeshType>(static_cast<int>(a_1) & static_cast<int>(a_2)); }


class MeshFilter : public Component
{
public:

	MeshFilter(Entity* a_pOwner);
	~MeshFilter();

	virtual void Update(float a_fDeltaTime) {};
	virtual void OnGUI();
	virtual bool OnDelete();
	virtual Component* Duplicate(Entity* a_owner);

	virtual void Save(std::ostream* a_outStream);

	void LoadModel();
	void Draw(Shader* a_shader, bool a_tessalation);

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
	std::string m_modelName;

	MeshType m_meshType;

	char m_modelTextbuff[256]; // For ImGui
	char m_shaderTextbuff[128]; // For ImGui


	// Imgui
	bool m_fileExplore;
};

#endif
