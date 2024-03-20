#include "TestGameLayer.h"

#include <imgui.h>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/// SHADER
const char* vertexShaderSource2D = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 colour;
    layout (location = 2) in vec2 uv;

    out vec3 vColour;
    out vec2 vUV;

    uniform vec2 offset;

    void main()
    {
        vec2 newPos = position.xy + offset;
        gl_Position = vec4(newPos.xy, position.z, 1.0);
        vColour = colour;
        vUV = uv;
    }
)glsl";

// Fragment shader source code
const char* fragmentShaderSource2D = R"glsl(
    #version 330 core
    out vec4 color;

    in vec3 vColour;
    in vec2 vUV;

    uniform sampler2D diffuse;

    void main()
    {
        vec4 texColour = texture(diffuse, vUV);
        color = texColour * vec4(vColour.xyz, 1.0f);
    }
)glsl";

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

    uniform sampler2D diffuse;

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
    m_test2DShader = Caramel::Shader::Create({ vertexShaderSource2D, fragmentShaderSource2D, {"offset"}});
    m_testTexture = Caramel::Texture::Create({ "Assets/wall.jpg", {Caramel::TextureType::T_2D} });
    Caramel::BufferLayout layout =
    {
        {Caramel::ShaderDataType::Float3, "position"},
        {Caramel::ShaderDataType::Float3, "colour"},
        {Caramel::ShaderDataType::Float2, "uv"}
    };

    {
        m_testTriangle = Caramel::VertexArray::Create();
        /// VERTEX
        float vertices[] = {
            -0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 0.0f,      0.0f, 0.0f, // Left  
             0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f,      1.0f, 0.0f, // Right 
             0.0f,  0.5f, 0.0f,      0.0f, 0.0f, 1.0f,      0.5f, 1.0f, // Top   
        };
        std::shared_ptr<Caramel::VertexBuffer> vbuffer = Caramel::VertexBuffer::Create({ vertices, sizeof(vertices) / sizeof(float)});
        vbuffer->SetLayout(layout);

        m_testTriangle->AddVertexBuffer(vbuffer);

        /// INDEX
        uint32_t indicies[] = { 0, 1, 2 };
        std::shared_ptr<Caramel::IndexBuffer> iBuffer = Caramel::IndexBuffer::Create({ indicies, sizeof(indicies) / sizeof(uint32_t)});
        m_testTriangle->SetIndexBuffer(iBuffer);
    }

    {
        float starVertices[] =
        {
            0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,    0.5f, 0.5f, // Center 0

            0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,    0.5f, 1.0f, // Top 1

            1.0f, 0.3f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 1.0f, // Right outer upper 2
            0.6f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,    1.0f, 0.0f, // Right outer lower 3

            0.3f, 0.3f, 0.0f,   0.0f, 1.0f, 0.0f,    0.8f, 1.0f, // Right inner upper 4
            0.3f, -0.2f, 0.0f,   0.0f, 1.0f, 0.0f,   0.8f, 0.8f, // Right inner lower 5

            0.0f, -0.4f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 0.3f, // Bottom 6

            -1.0f, 0.3f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f  // Left outer upper 7 
            - 0.6f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Left outer lower 8

            -0.3f, 0.3f, 0.0f,   1.0f, 0.0f, 0.0f,   0.2f, 1.0f, // Left inner upper 9
            -0.3f, -0.2f, 0.0f,   1.0f, 0.0f, 0.0f,  0.2f, 0.8f// Left inner lower 10
        };

        // Indices array
        unsigned int starIndices[] =
        {
            0, 1, 4,
            0, 4, 2,
            0, 2, 5,
            0, 5, 3,
            0, 3, 6,

            0, 6, 8,
            0, 8, 10,
            0, 10, 7,
            0, 7, 9,
            0, 9, 1
        };

        m_testStar = Caramel::VertexArray::Create();
        std::shared_ptr<Caramel::VertexBuffer> vBuffer = Caramel::VertexBuffer::Create({ starVertices, sizeof(starVertices) / sizeof(float)});
        vBuffer->SetLayout(layout);
        m_testStar->AddVertexBuffer(vBuffer);

        std::shared_ptr<Caramel::IndexBuffer> iBuffer = Caramel::IndexBuffer::Create({ starIndices, sizeof(starIndices) / sizeof(uint32_t)});
        m_testStar->SetIndexBuffer(iBuffer);
    }


    m_test3DShader = Caramel::Shader::Create({ vertexShaderSource3D, fragmentShaderSource3D, {"transform", "diffuse"}});
    Caramel::BufferLayout layout3D =
    {
        {Caramel::ShaderDataType::Float3, "position"}
    };

    {
        m_testCube = Caramel::VertexArray::Create();

        float cube[] =
        {
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, -0.5f,
            -0.5f, 0.5f, 0.5f,
            0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, 0.5f,
        };

        auto vBuffer = Caramel::VertexBuffer::Create({ cube, sizeof(cube) / sizeof(float)});
        vBuffer->SetLayout(layout3D);
        m_testCube->AddVertexBuffer(vBuffer);

        uint32_t iCube[] =
        {
            0, 1, 2,
            1, 3, 4,
            5, 6, 3,
            7, 3, 6,
            2, 4, 7,
            0, 7, 6,
            0, 5, 1,
            1, 5, 3,
            5, 0, 6,
            7, 4, 3,
            2, 1, 4,
            0, 2, 7,
        };

        auto iBuffer = Caramel::IndexBuffer::Create({ iCube, sizeof(iCube) / sizeof(uint32_t)});
        m_testCube->SetIndexBuffer(iBuffer);
    }

    m_testModel = Caramel::VertexArray::Create();
    

    Assimp::Importer importer;
    const std::string modelPath = "Assets/Models/ship.obj";
    const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        // Error handling
        CL_CORE_FATAL("ERROR::ASSIMP::{}", importer.GetErrorString());
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

        std::vector<float> vertices (vertCount * 3);
        std::vector<uint32_t> indices (indicesCount);

        size_t vInsert = 0;
        size_t iInsert = 0;

        for (int i = 0; i < scene->mNumMeshes; ++i)
        {
            memcpy(&vertices[vInsert], scene->mMeshes[i]->mVertices, scene->mMeshes[i]->mNumVertices * sizeof(float) * 3);
            vInsert += scene->mMeshes[i]->mNumVertices * 3;

            for (int j = 0; j < scene->mMeshes[i]->mNumFaces; ++j)
            {
                memcpy(&indices[iInsert], scene->mMeshes[i]->mFaces[j].mIndices, sizeof(uint32_t) * 3);
                iInsert += 3;
            }
        }

        uint32_t curCount = vertices.size();
        auto vBuffer = Caramel::VertexBuffer::Create({ &vertices[0], curCount });
        vBuffer->SetLayout(layout3D);
        m_testModel->AddVertexBuffer(vBuffer);

        curCount = indices.size();
        auto iBuffer = Caramel::IndexBuffer::Create({ &indices[0], curCount });
        m_testModel->SetIndexBuffer(iBuffer);
    }
}

void TestGameLayer::OnImGuiRender()
{
    ImGui::Begin("Controls");

    ImGui::DragFloat("Spin speed", &m_spinSpeed);
    ImGui::DragFloat("Angle", &m_currrentAngle, 1.0f, 0.001f, 360.0f, "%.3f");
    ImGui::DragFloat("Radius", &m_radius, 0.01f, 0.001f);
    ImGui::Checkbox("Auto Spin", &m_spin);

    ImGui::DragFloat("Cube spin speed", &m_cubeScaleSpeed, 0.1f);
    ImGui::DragFloat("FOV", &m_fov, 0.01f);
    ImGui::DragFloat("Near plane", &m_nearPlane, 0.01f);
    ImGui::DragFloat("Far plane", &m_farPlane, 0.01f);

    ImGui::End();
}

double degreesToRadians(double degrees)
{
    return degrees * 3.14159265358979323846 / 180.0;
}

void TestGameLayer::SetPointOnCircle(float radius, float angleInDegrees)
{
    // Convert angle from degrees to radians
    float angleInRadians = degreesToRadians(angleInDegrees);

    // Calculate x and y using the polar coordinates to Cartesian coordinates conversion
    m_testOffset[0] = radius * cos(angleInRadians); // x-coordinate
    m_testOffset[1] = radius * sin(angleInRadians); // y-coordinate
}

void TestGameLayer::OnUpdate(Caramel::Timestep ts)
{
    m_test2DShader->Bind();
    if (m_spin)
    {
        m_currrentAngle += m_spinSpeed * ts;
        
    }
    SetPointOnCircle(m_radius, m_currrentAngle);
    m_test2DShader->SetValue("offset", Caramel::ShaderDataType::Float2, &m_testOffset);
    m_testTexture->Bind(0);
    Caramel::Renderer::Submit(m_testTriangle);


    m_cubeScale += m_cubeScaleSpeed * ts;

    float tanHalfFOV = tanf(degreesToRadians(m_fov / 2));
    float d = 1 / tanHalfFOV;

    auto wind = Caramel::Application::Get()->GetWindow();
    float ar = (float)wind->GetWidth() / (float)wind->GetHeight();

    float nearFarRange = m_nearPlane - m_farPlane;

    float projA = (-m_farPlane - m_nearPlane) / nearFarRange;
    float projB = 2.0f * m_farPlane * m_nearPlane / nearFarRange;

    aiMatrix4x4 rot
    {
        cosf(m_cubeScale), 0.0f, -sinf(m_cubeScale), 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        sinf(m_cubeScale), 0.0f, cosf(m_cubeScale), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    aiMatrix4x4 trans
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f,
        0.0f, 0.0f, 1.0f, 5.0f,
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
    Caramel::Renderer::Submit(m_testModel);
}
