#include "clpch.h"
#include "RenderAPI.h"

#include "Core/Application.h"


#include "Platform/RenderAPI/DX12/RenderAPI_DX12.h"
#include <Core/Log.h>

namespace Caramel
{
	RenderAPI* RenderAPI::Create(WindowRenderAPI api)
	{
		switch (api)
		{
		case WindowRenderAPI::DirectX12: return new RenderAPI_DX12();
		}

		CL_CORE_NOT_IMPLEMENTED;
		return nullptr;
	}
}

