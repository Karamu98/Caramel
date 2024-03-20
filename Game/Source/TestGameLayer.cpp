#include "TestGameLayer.h"

#include <imgui.h>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


const char* vertexShaderSource3D = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 position;

    uniform mat4 transform;

    out vec3 vPos;

    void main()
    {
        vec4 worldPos = transform * vec4(position.xyz, 1.0);
        vPos = position.xyz;
        gl_Position = worldPos;
    }
)glsl";

// Fragment shader source code
const char* fragmentShaderSource3D = R"glsl(
    #version 330 core
    out vec4 color;

    in vec3 vPos;

    void main()
    {
        color = vec4(vPos.xyz, 1.0f);
    }
)glsl";

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

void TestGameLayer::OnAttach()
{
    m_test3DShader = Caramel::Shader::Create({ vertexShaderSource3D, fragmentShaderSource3D, {"transform"} });
    Caramel::BufferLayout layout3D =
    {
        {Caramel::ShaderDataType::Float3, "position"}
    };

    Assimp::Importer importer;
    const std::string modelPath = "Assets/Models/ship.obj";
    const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        CL_CORE_FATAL("Assimp error: {}", importer.GetErrorString());
        return;
    }
    else
    {
        std::string info = std::format("Loaded model ({0})\n\tMeshes: {1}\n\tSkeletons: {2}\n\tAnimations: {3}\n\tTextures: {4}", modelPath, scene->mNumMeshes, scene->mNumSkeletons, scene->mNumAnimations, scene->mNumTextures);
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

        uint32_t vertCount = 0;
        uint32_t indicesCount = 0;
        for (int i = 0; i < scene->mNumMeshes; ++i)
        {
            vertCount += scene->mMeshes[i]->mNumVertices;
            indicesCount += scene->mMeshes[i]->mNumFaces * 3;
        }



        for (int i = 0; i < scene->mNumMeshes; ++i)
        {
            m_testMeshes.push_back(Caramel::VertexArray::Create());
            std::vector<float> vertices;
            std::vector<uint32_t> indices;

            for (int j = 0; j < scene->mMeshes[i]->mNumVertices; ++j)
            {
                vertices.push_back(scene->mMeshes[i]->mVertices[j].x);
                vertices.push_back(scene->mMeshes[i]->mVertices[j].y);
                vertices.push_back(scene->mMeshes[i]->mVertices[j].z);
            }

            for (int j = 0; j < scene->mMeshes[i]->mNumFaces; ++j)
            {
                indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[0]);
                indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[1]);
                indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[2]);
            }

            uint32_t curCount = vertices.size();
            auto vBuffer = Caramel::VertexBuffer::Create({ &vertices[0], curCount });
            vBuffer->SetLayout(layout3D);
            m_testMeshes.back()->AddVertexBuffer(vBuffer);

            curCount = indices.size();
            auto iBuffer = Caramel::IndexBuffer::Create({ &indices[0], curCount });
            m_testMeshes.back()->SetIndexBuffer(iBuffer);
        }
    }
}

void TestGameLayer::OnImGuiRender()
{
    ImGui::Begin("Controls");

    ImGui::DragFloat("Model spin speed", &m_modelSpinSpeed, 0.01f);
    ImGui::Spacing();
    ImGui::DragFloat("FOV", &m_fov, 0.1f);
    ImGui::DragFloat("Near plane", &m_nearPlane, 0.01f);
    ImGui::DragFloat("Far plane", &m_farPlane, 0.01f);

    ImGui::End();
}

double degreesToRadians(double degrees)
{
    return degrees * 3.14159265358979323846 / 180.0;
}

void TestGameLayer::OnUpdate(Caramel::Timestep ts)
{
    m_curModelSpin += m_modelSpinSpeed * ts;

    float tanHalfFOV = tanf(degreesToRadians(m_fov / 2));
    float d = 1 / tanHalfFOV;

    auto wind = Caramel::Application::Get()->GetWindow();
    float ar = (float)wind->GetWidth() / (float)wind->GetHeight();

    float nearFarRange = m_nearPlane - m_farPlane;

    float projA = (-m_farPlane - m_nearPlane) / nearFarRange;
    float projB = 2.0f * m_farPlane * m_nearPlane / nearFarRange;

    aiMatrix4x4 rot
    {
        cosf(m_curModelSpin), 0.0f, -sinf(m_curModelSpin), 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        sinf(m_curModelSpin), 0.0f, cosf(m_curModelSpin), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    aiMatrix4x4 trans
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -5.0f,
        0.0f, 0.0f, 1.0f, 10.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };

    aiMatrix4x4 proj
    {
        d/ar, 0.0f, 0.0f, 0.0f,
        0.0f, d, 0.0f, 0.0f,
        0.0f, 0.0f, projA, projB,
        0.0f, 0.0f, 1.0f, 0.0f,
    };

    aiMatrix4x4 finalMat = proj * trans * rot;



    m_test3DShader->Bind();
    m_test3DShader->SetValue("transform", Caramel::ShaderDataType::Mat4, &finalMat.a1);
    //Caramel::Renderer::Submit(m_testCube);
    for (const auto& mesh : m_testMeshes)
    {
        Caramel::Renderer::Submit(mesh);
    }
}
