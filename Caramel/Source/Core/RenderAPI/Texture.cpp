#include "clpch.h"
#include "Texture.h"

#include "Core/Application.h"

#include "Platform/RenderAPI/OpenGL/Texture_OpenGL.h"

namespace Caramel
{
	std::shared_ptr<Texture> Texture::Create(const TextureParams& params)
	{
		switch (Application::GetRenderAPI())
		{
		case WindowRenderAPI::OpenGL: return std::make_shared<Texture_OpenGL>(params);
		}

		return nullptr;
	}
}

