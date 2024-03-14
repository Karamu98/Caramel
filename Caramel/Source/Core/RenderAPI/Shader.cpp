#include "clpch.h"
#include "Shader.h"

#include "Core/Application.h"

#include "Platform/RenderAPI/OpenGL/Shader_OpenGL.h"

namespace Caramel
{
	std::shared_ptr<Shader> Caramel::Shader::Create(const ShaderParams& params)
	{
		switch (Application::GetRenderAPI())
		{
		case WindowRenderAPI::OpenGL: return std::make_shared<Shader_OpenGL>(params);
		}

		CL_CORE_ASSERT(false);
		return nullptr;
	}
}

