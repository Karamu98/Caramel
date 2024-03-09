#include "clpch.h"
#include "ImGuiImpl.h"

#include "Application.h"

#include <Platform/ImGui/ImGuiImpl_DX12.h>


namespace Caramel
{
	ImGuiImpl* Caramel::ImGuiImpl::Create()
	{
		switch (Application::GetRenderAPI())
		{
		case WindowRenderAPI::DirectX12: return new ImGuiImpl_DX12();
		}
		return nullptr;
	}
}

