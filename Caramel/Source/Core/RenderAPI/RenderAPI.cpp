#include "clpch.h"
#include "RenderAPI.h"

#include "Core/Application.h"


#include "Platform/RenderAPI/OpenGL/RenderAPI_OpenGL.h"
#include <Core/Log.h>

#ifdef CL_PLATFORM_WINDOWS
#include "Platform/RenderAPI/DX12/RenderAPI_DX12.h"
#endif // CL_PLATFORM_WINDOWS


namespace Caramel
{
	RenderAPI* RenderAPI::Create(WindowRenderAPI api)
	{
		switch (api)
		{
#ifdef CL_PLATFORM_WINDOWS
		case WindowRenderAPI::DirectX12: return new RenderAPI_DX12();
#endif
		case WindowRenderAPI::OpenGL: return new RenderAPI_OpenGL();
		}

		CL_CORE_NOT_IMPLEMENTED;
		return nullptr;
	}
}

