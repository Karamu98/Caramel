#include "clpch.h"
#include "Model.h"

#include <Core/Application.h>

#include <Platform/RenderAPI/OpenGL/Model_OpenGL.h>

namespace Caramel
{
    std::shared_ptr<Model> Model::Create(const ModelParams& params)
    {
        switch (Application::GetRenderAPI())
        {
        case WindowRenderAPI::OpenGL: return std::make_shared<Model_OpenGL>(params);
        }

        return nullptr;
    }
}

