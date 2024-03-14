#include "clpch.h"
#include "Buffer.h"

#include <Core/Application.h>

#include <Platform/RenderAPI/DX12/Buffer_DX12.h>
#include <Platform/RenderAPI/OpenGL/Buffer_OpenGL.h>

namespace Caramel
{
	std::shared_ptr<VertexBuffer> Caramel::VertexBuffer::Create(const VertexBufferParams& params)
	{
		switch (Application::GetRenderAPI())
		{
		case WindowRenderAPI::OpenGL: return std::make_shared<VertexBuffer_OpenGL>(params);
		//case WindowRenderAPI::DirectX12: return new VertexBuffer_DX12(params);
		}
		return nullptr;
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(const IndexBufferParams& params)
	{
		switch (Application::GetRenderAPI())
		{
		case WindowRenderAPI::OpenGL: return std::make_shared<IndexBuffer_OpenGL>(params);
		//case WindowRenderAPI::DirectX12: return new IndexBuffer_DX12(params);
		}
		return nullptr;
	}

	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		switch (Application::GetRenderAPI())
		{
		case WindowRenderAPI::OpenGL: return std::make_shared<VertexArray_OpenGL>();
			//case WindowRenderAPI::DirectX12: return new IndexBuffer_DX12(params);
		}
		return nullptr;
	}
}

