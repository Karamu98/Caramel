#pragma once

#include "Core/RenderAPI/Buffer.h"

namespace Caramel
{
	class VertexBuffer_OpenGL : public VertexBuffer
	{
	public:
		VertexBuffer_OpenGL(const VertexBufferParams& params);
		virtual ~VertexBuffer_OpenGL();

		// Inherited via VertexBuffer
		virtual void SetLayout(const BufferLayout& layout) override;
		virtual const BufferLayout& GetLayout() override;
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual uint32_t GetVertexCount() override;

	private:		

		uint32_t m_id;
		BufferLayout m_layout;
		uint32_t m_count;
	};

	class IndexBuffer_OpenGL : public IndexBuffer
	{
	public:
		IndexBuffer_OpenGL(const IndexBufferParams& params);
		virtual ~IndexBuffer_OpenGL();

		// Inherited via IndexBuffer
		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const override;

	private:

		uint32_t m_id;
		uint32_t m_count;
	};

	class VertexArray_OpenGL : public VertexArray
	{
	public:
		VertexArray_OpenGL();
		virtual ~VertexArray_OpenGL();

		// Inherited via VertexArray
		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;
		virtual bool HasIndexBuffer() override;
		virtual uint32_t GetTotalVertexCount() override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override;

	private:

		uint32_t m_id;
		std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
		uint32_t m_totalVerticies = 0;
	};
}


