#pragma once

#include <Core/RenderAPI/Buffer.h>

namespace Caramel
{
	class VertexBuffer_DX12 : public VertexBuffer
	{
	public:
		virtual ~VertexBuffer_DX12();

		// Inherited via VertexBuffer
		void Bind() const override;
		void Unbind() const override;

	private:
		friend class VertexBuffer;
		VertexBuffer_DX12(const VertexBufferParams& params);
	};

	class IndexBuffer_DX12 : public IndexBuffer
	{
	public:
		virtual ~IndexBuffer_DX12();

		// Inherited via IndexBuffer
		void Bind() const override;
		void Unbind() const override;

	private:
		friend class IndexBuffer;
		IndexBuffer_DX12(const IndexBufferParams& params);
	};
}


