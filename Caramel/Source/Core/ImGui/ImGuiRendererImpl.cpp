#include "clpch.h"
#include "ImGuiRendererImpl.h"

#include <Core/Application.h>

#include <Platform/ImGuiImpl/DX12/ImGuiImpl_DX12.h>
#include <Platform/ImGuiImpl/OpenGL/ImGuiRendererImpl_OpenGL.h>


namespace Caramel
{
	ImGuiRendererImpl* Caramel::ImGuiRendererImpl::Create()
	{
		switch (Application::GetRenderAPI())
		{
		case WindowRenderAPI::DirectX12: return new ImGuiRendererImpl_DX12();
		case WindowRenderAPI::OpenGL: return new ImGuiRendererImpl_OpenGL();
		}
		return nullptr;
	}
}

