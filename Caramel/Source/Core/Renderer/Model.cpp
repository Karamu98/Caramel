#include "clpch.h"
#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Core/RenderAPI/Buffer.h>
#include <Core/Renderer/Material.h>


std::string aiIllumModelToString(unsigned int type)
{
    switch (type) 
    {
    case 0: return "NoShading";
    case 1: return "Gouraud";
    case 2: return "Phong";
    default: return "Unknown";
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
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
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

        newModel->processNode(scene->mRootNode, scene);
        //newModel->printDebug(scene->mRootNode, scene);

        newModel->createLUT();

        return newModel;
    }

    void Model::processNode(aiNode* node, const aiScene* scene)
    {
        // process each mesh located at the current node
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_meshes.emplace_back(mesh, scene, m_path);
        }

        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    void Model::printDebug(aiNode* node, const aiScene* scene)
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
            info += GetMaterialInfo(curMat);
        }
        CL_CORE_INFO(info.c_str());
    }

    void Model::createLUT()
    {
        for (auto& mesh : m_meshes)
        {
            MeshByMaterials[mesh.Material].push_back(&mesh);
        }
    }

    std::string Model::GetMaterialInfo(const aiMaterial* material)
    {
        std::string outInfo{};
        for (int i = 0; i < material->mNumProperties; ++i) 
        {
            aiMaterialProperty* curProp = material->mProperties[i];

            outInfo += std::format("\n\t\t\tProperty {0} ({1}):", std::to_string(i), std::string(curProp->mKey.C_Str()));
            if (curProp->mSemantic != aiTextureType_NONE)
            {
                outInfo += std::format("\n\t\t\t\tSemantic: {}", magic_enum::enum_name((aiTextureType)curProp->mSemantic));
                outInfo += "\n\t\t\t\tIndex: " + std::to_string(curProp->mIndex) + ", ";
            }

            if (curProp->mKey == aiString("$mat.shadingm"))
            {
                std::vector<uint32_t> values;
                GetValues(curProp->mData, curProp->mDataLength, values);
                outInfo += "\n\t\t\t\tType: Shading mode";
                outInfo += std::format("\n\t\t\t\t\t{1} - {0}", magic_enum::enum_name((aiShadingMode)values[0]),  values[0]);
                continue;
            }

            if (curProp->mKey == aiString("$mat.illum"))
            {
                std::vector<uint32_t> values;
                GetValues(curProp->mData, curProp->mDataLength, values);
                outInfo += "\n\t\t\t\tType: Illumination model";
                outInfo += std::format("\n\t\t\t\t\t{1} - {0}", aiIllumModelToString(values[0]), values[0]);
                continue;
            }

            //outInfo += "\n\t\t\tType: " + std::to_string(property->mType) + ", ";
            //outInfo += "\n\t\t\t\tData length: " + std::to_string(curProp->mDataLength);

            switch (curProp->mType) 
            {
            case aiPTI_Float:
            {
                std::vector<float> values;
                GetValues(curProp->mData, curProp->mDataLength, values);

                std::string keyAsStr(curProp->mKey.C_Str());
                if (keyAsStr.starts_with("$clr."))
                {
                    std::vector<float> colours(values.begin(), values.end());
                    for (auto& val : colours)
                    {
                        val *= 255;
                    }

                    outInfo += "\n\t\t\t\tType: Colour";
                    outInfo += std::format("\n\t\t\t\t\tData: {3}\033[48;2;{0};{1};{2}m\t-        -\033[0m\033[32m", colours[0], colours[1], colours[2], FormatVector(values));
                }
                else
                {
                    outInfo += "\n\t\t\t\tType: Float";
                    outInfo += "\n\t\t\t\t\tData: " + FormatVector(values);
                }

                continue;
            }

            case aiPTI_Integer:
            {
                std::vector<uint32_t> values;
                GetValues(curProp->mData, curProp->mDataLength, values);
                outInfo += "\n\t\t\t\tType: Int";
                outInfo += "\n\t\t\t\t\tData: " + FormatVector(values);
                continue;
            }
            case aiPTI_String:
            {
                outInfo += "\n\t\t\t\tType: String";
                std::vector<char> values;
                GetValues(&curProp->mData[4], curProp->mDataLength - 5, values);
                std::string value(values.begin(), values.end());
                outInfo += std::format("\n\t\t\t\t\tData: {}", value);
                continue;
            }

            case aiPTI_Buffer:
            {
                std::vector<char> values;
                GetValues(curProp->mData, curProp->mDataLength, values);
                outInfo += "\n\t\t\t\tType: Buffer";
                outInfo += "\n\t\t\t\t\tData: " + FormatVector(values);
                continue;
            }
            default:
                outInfo += "\n\t\t\t\tType: Unknown";
                outInfo += "\n\t\t\t\t\tData: [Unknown type]";
                continue;
            }
        }
        return outInfo;
    }

    Mesh::Mesh(aiMesh* mesh, const aiScene* scene, const std::string& modelPath)
    {
        // walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            vertex.Position =
            {
                mesh->mVertices[i].x,
                mesh->mVertices[i].y,
                mesh->mVertices[i].z
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


        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::string modelID = modelPath + material->GetName().C_Str();
        Material = Material::Get(modelID);
        if (Material == nullptr)
        {
            Material = Material::CreateFromAssimp(modelPath, modelID, material);
        }

    }
}