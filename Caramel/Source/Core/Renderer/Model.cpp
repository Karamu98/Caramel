#include "clpch.h"
#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Core/RenderAPI/Buffer.h>

inline std::string aiTypeToString(aiTextureType type)
{
    switch (type)
    {
    case aiTextureType_DIFFUSE: return "Diffuse";
    case aiTextureType_SPECULAR: return "Specular";
    case aiTextureType_AMBIENT: return "Ambient";
    case aiTextureType_EMISSIVE: return "Emissive";
    case aiTextureType_HEIGHT: return "Height";
    case aiTextureType_NORMALS: return "Normals";
    case aiTextureType_SHININESS: return "Shininess";
    case aiTextureType_OPACITY: return "Opacity";
    case aiTextureType_DISPLACEMENT: return "Displacement";
    case aiTextureType_LIGHTMAP: return "Lightmap";
    case aiTextureType_REFLECTION: return "Reflection";
    case aiTextureType_BASE_COLOR: return "Base_Color";
    case aiTextureType_NORMAL_CAMERA: return "Normal_Camera";
    case aiTextureType_EMISSION_COLOR: return "Emission_Color";
    case aiTextureType_METALNESS: return "Metalness";
    case aiTextureType_DIFFUSE_ROUGHNESS: return "Diffuse_Roughness";
    case aiTextureType_AMBIENT_OCCLUSION: return "Ambient_Occlusion";
    case aiTextureType_SHEEN: return "Sheen";
    case aiTextureType_CLEARCOAT: return "Clearcoat";
    case aiTextureType_TRANSMISSION: return "Transmission";
    default:
    case aiTextureType_UNKNOWN: return "Unknown";
    }
}

namespace Caramel
{
	std::shared_ptr<Model> Model::GetOrLoad(const std::string& path)
	{
        return Load(path);
	}

    std::shared_ptr<Model> Model::Load(const std::string& path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            CL_CORE_FATAL("Assimp error: {}", importer.GetErrorString());
            return nullptr;
        }

        std::shared_ptr<Model> newModel = std::make_shared<Model>();
        newModel->m_path = path;

        uint32_t vertCount = 0;
        for (int i = 0; i < scene->mNumMeshes; ++i)
        {
            vertCount += scene->mMeshes[i]->mNumVertices;
        }
        newModel->m_vertCount = vertCount;

        newModel->ProcessNode(scene->mRootNode, scene);

        return newModel;
    }

    void Model::ProcessNode(aiNode* node, const aiScene* scene)
    {
        // process each mesh located at the current node
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_meshes.emplace_back(mesh, scene);
        }

        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene);
        }
    }

    void Model::PrintDebug(aiNode* node, const aiScene* scene)
    {
        std::string info = std::format("Loaded model ({0})\n\tMeshes: {1}\n\tSkeletons: {2}\n\tAnimations: {3}\n\tTextures: {4}", m_path, scene->mNumMeshes, scene->mNumSkeletons, scene->mNumAnimations, scene->mNumTextures);
        for (int i = 0; i < scene->mNumTextures; ++i)
        {
            info += std::format("\n\t\t({1} x {2}){0}", scene->mTextures[i]->mFilename.C_Str(), scene->mTextures[i]->mWidth, scene->mTextures[i]->mHeight);
        }
        info += std::format("\n\tMaterials: {0}", scene->mNumMaterials);
        for (int i = 0; i < scene->mNumMaterials; ++i)
        {
            const aiMaterial* curMat = scene->mMaterials[i];
            info += std::format("\n\t\t({0}) properties: {1}", curMat->GetName().C_Str(), curMat->mNumProperties);

            for (int j = 0; j < curMat->mNumProperties; ++j)
            {
                const aiMaterialProperty* curProp = curMat->mProperties[j];
                if (curProp->mSemantic != aiTextureType_NONE)
                {
                    aiString texPath;
                    curMat->GetTexture((aiTextureType)curProp->mSemantic, curProp->mIndex, &texPath);
                    info += std::format("\n\t\t\tTexture property ({2}):\n\t\t\t\tType: {0}\n\t\t\t\tPath: {1}", aiTypeToString((aiTextureType)curProp->mSemantic), texPath.C_Str(), curProp->mKey.C_Str());
                }
                else
                {
                    //info += std::format("\n\t\t\t{}", curProp->mKey.C_Str());
                }
            }
        }
        CL_CORE_INFO(info.c_str());
    }

    Mesh::Mesh(aiMesh* mesh, const aiScene* scene)
    {
        // walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            // positions
            Vertex vertex =
            {
                {
                    mesh->mVertices[i].x,
                    mesh->mVertices[i].y,
                    mesh->mVertices[i].z
                },
            };

            // normals
            if (mesh->HasNormals())
            {
                vertex.Normal =
                {
                    mesh->mNormals[i].x,
                    mesh->mNormals[i].y,
                    mesh->mNormals[i].z
                };
            }

            // texture coordinates
            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                vertex.UV =
                {
                    mesh->mTextureCoords[0][i].x,
                    mesh->mTextureCoords[0][i].y
                };
            }

            Verticies.push_back(vertex);
        }

        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                Indicies.push_back(face.mIndices[j]);
            }
        }

        static BufferLayout layout =
        {
            {ShaderDataType::Float3, "v_Position"},
            {ShaderDataType::Float3, "v_Normal"},
            {ShaderDataType::Float2, "v_UV"}
        };

        Data = VertexArray::Create();

        float* start = (float*)&Verticies[0];
        uint32_t count = Verticies.size();
        std::shared_ptr<VertexBuffer> vBuffer = VertexBuffer::Create({ start, sizeof(Vertex), count});
        vBuffer->SetLayout(layout);
        Data->AddVertexBuffer(vBuffer);

        count = Indicies.size();
        std::shared_ptr<IndexBuffer> iBuffer = IndexBuffer::Create({ &Indicies[0], count });
        Data->SetIndexBuffer(iBuffer);


        //// process materials
        //aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        //// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        //// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        //// Same applies to other texture as the following list summarizes:
        //// diffuse: texture_diffuseN
        //// specular: texture_specularN
        //// normal: texture_normalN

        //// 1. diffuse maps
        //vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        //textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        //// 2. specular maps
        //vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        //textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        //// 3. normal maps
        //std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        //textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        //// 4. height maps
        //std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        //textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    }
}