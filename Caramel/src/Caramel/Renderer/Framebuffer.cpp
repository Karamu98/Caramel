#include "clpch.h"
#include "Caramel/Renderer/Framebuffer.h"

#include "Caramel/Platform/OpenGL/OpenGLFramebuffer.h"

namespace Caramel {

	Caramel::Framebuffer* Framebuffer::Create(uint32_t width, uint32_t height, FramebufferFormat format)
	{
		Caramel::Framebuffer* result = nullptr;

		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:		return nullptr;
			case RendererAPIType::OpenGL:	result = new OpenGLFramebuffer(width, height, format);
		}
		FramebufferPool::GetGlobal()->Add(result);
		return result;
	}

	FramebufferPool* FramebufferPool::s_Instance = new FramebufferPool;

	FramebufferPool::FramebufferPool(uint32_t maxFBs /* = 32 */)
	{

	}

	FramebufferPool::~FramebufferPool()
	{
		
	}

	std::weak_ptr<Framebuffer> FramebufferPool::AllocateBuffer()
	{
		// m_Pool.push_back();
		return std::weak_ptr<Framebuffer>();
	}

	void FramebufferPool::Add(Framebuffer* framebuffer)
	{
		m_Pool.push_back(framebuffer);
	}

}