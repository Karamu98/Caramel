#pragma once

#include "Caramel/Renderer/Buffer.h"

namespace Caramel 
{
	//////////////////////////////////////////////////////////////////////////////////
	// VertexBuffer
	//////////////////////////////////////////////////////////////////////////////////

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(unsigned int size);
		virtual ~OpenGLVertexBuffer();

		virtual void SetData(void* buffer, unsigned int size, unsigned int offset = 0);
		virtual void Bind() const;

		virtual unsigned int GetSize() const { return m_Size; }
		virtual RendererID GetRendererID() const { return m_RendererID; }
	private:
		RendererID m_RendererID;
		unsigned int m_Size;
	};

	//////////////////////////////////////////////////////////////////////////////////
	// IndexBuffer
	//////////////////////////////////////////////////////////////////////////////////

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(unsigned int size);
		virtual ~OpenGLIndexBuffer();

		virtual void SetData(void* buffer, unsigned int size, unsigned int offset = 0);
		virtual void Bind() const;

		virtual uint32_t GetCount() const { return m_Size / sizeof(uint32_t); }

		virtual unsigned int GetSize() const { return m_Size; }
		virtual RendererID GetRendererID() const { return m_RendererID; }
	private:
		RendererID m_RendererID;
		unsigned int m_Size;
	};

}