#include "clpch.h"
#include "Texture.h"

#include "Core/Application.h"


namespace Caramel
{
	std::shared_ptr<Texture> Texture::Create()
	{
		switch (Application::GetRenderAPI())
		{
		//case WindowRenderAPI::OpenGL: return new
		}

		return nullptr;
	}
}

