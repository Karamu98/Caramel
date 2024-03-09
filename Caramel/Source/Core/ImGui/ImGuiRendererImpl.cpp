#include "clpch.h"
#include "ImGuiRendererImpl.h"

#include <Core/Application.h>

#include <Platform/ImGuiImpl/DX12/ImGuiImpl_DX12.h>


namespace Caramel
{
	ImGuiRendererImpl* Caramel::ImGuiRendererImpl::Create()
	{
		switch (Application::GetRenderAPI())
		{
		case WindowRenderAPI::DirectX12: return new ImGuiRendererImpl_DX12();
		}
		return nullptr;
	}
}

