#pragma once

#include "Caramel/Renderer/RendererAPI.h"

namespace Caramel 
{
	enum class FramebufferFormat
	{
		None    = 0,
		RGBA8   = 1,
		RGBA16F = 2
	};

	class Framebuffer
	{
	public:
		static Framebuffer* Create(uint32_t width, uint32_t height, FramebufferFormat format);

		virtual ~Framebuffer() {}
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual void BindTexture(uint32_t slot = 0) const = 0;

		virtual RendererID GetRendererID() const = 0;
		virtual RendererID GetColorAttachmentRendererID() const = 0;
		virtual RendererID GetDepthAttachmentRendererID() const = 0;
	};

	class FramebufferPool final
	{
	public:
		FramebufferPool(uint32_t maxFBs = 32);
		~FramebufferPool();

		std::weak_ptr<Framebuffer> AllocateBuffer();
		void Add(Framebuffer* framebuffer);

		const std::vector<Framebuffer*>& GetAll() const { return m_Pool; }

		inline static FramebufferPool* GetGlobal() { return s_Instance; }
	private:
		std::vector<Framebuffer*> m_Pool;

		static FramebufferPool* s_Instance;
	};

}