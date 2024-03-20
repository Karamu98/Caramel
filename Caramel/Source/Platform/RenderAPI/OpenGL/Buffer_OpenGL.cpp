#include "clpch.h"
#include "Buffer_OpenGL.h"


#include <glad/glad.h>

namespace Caramel
{
	static inline GLenum ShaderTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4:
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
		{
			return GL_FLOAT;
		}

		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
		{
			return GL_INT;
		}

		case ShaderDataType::Bool:
		{
			return GL_BOOL;
		}


		}
	}


	VertexBuffer_OpenGL::~VertexBuffer_OpenGL()
	{
		glDeleteBuffers(1, &m_id);
	}

	void VertexBuffer_OpenGL::SetLayout(const BufferLayout& layout)
	{
		m_layout = layout;
	}

	const BufferLayout& VertexBuffer_OpenGL::GetLayout()
	{
		return m_layout;
	}

	VertexBuffer_OpenGL::VertexBuffer_OpenGL(const VertexBufferParams& params)
	{
		m_count = params.Count;

		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferData(GL_ARRAY_BUFFER, params.Count * sizeof(float), params.Verticies, GL_STATIC_DRAW);
	}

	void VertexBuffer_OpenGL::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	void VertexBuffer_OpenGL::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	uint32_t VertexBuffer_OpenGL::GetVertexCount()
	{
		return m_count;
	}

	IndexBuffer_OpenGL::IndexBuffer_OpenGL(const IndexBufferParams& params) : m_count(params.Count)
	{
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, params.Count * sizeof(uint32_t), params.Indicies, GL_STATIC_DRAW);
	}

	IndexBuffer_OpenGL::~IndexBuffer_OpenGL()
	{
		glDeleteBuffers(1, &m_id);
	}

	void IndexBuffer_OpenGL::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	}

	void IndexBuffer_OpenGL::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	uint32_t IndexBuffer_OpenGL::GetCount() const
	{
		return m_count;
	}

	VertexArray_OpenGL::VertexArray_OpenGL()
	{
		glCreateVertexArrays(1, &m_id);
	}

	VertexArray_OpenGL::~VertexArray_OpenGL()
	{
		glDeleteVertexArrays(1, &m_id);
	}

	void VertexArray_OpenGL::Bind() const
	{
		glBindVertexArray(m_id);
	}

	void VertexArray_OpenGL::Unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray_OpenGL::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		CL_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size() != 0, "Vertex buffer has no layout");
		glBindVertexArray(m_id);
		vertexBuffer->Bind();

		m_totalVerticies += vertexBuffer->GetVertexCount();

		uint32_t i = 0;
		const BufferLayout& layout = vertexBuffer->GetLayout();
		for (const auto& ele : layout)
		{
			int count = GetTypeComponentCount(ele.Type);
			GLenum baseType = ShaderTypeToOpenGLBaseType(ele.Type);
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i,
				GetTypeComponentCount(ele.Type),
				ShaderTypeToOpenGLBaseType(ele.Type),
				ele.Normalised ? GL_TRUE : GL_FALSE, 
				layout.GetStride(), 
				(const void*)ele.Offset);
			++i;
		}

		m_vertexBuffers.push_back(vertexBuffer);
	}

	void VertexArray_OpenGL::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_id);
		indexBuffer->Bind();

		m_indexBuffer = indexBuffer;
	}

	bool VertexArray_OpenGL::HasIndexBuffer()
	{
		return m_indexBuffer != nullptr;
	}

	uint32_t VertexArray_OpenGL::GetTotalVertexCount()
	{
		return m_totalVerticies;
	}

	const std::vector<std::shared_ptr<VertexBuffer>>& VertexArray_OpenGL::GetVertexBuffers() const
	{
		return m_vertexBuffers;
	}

	const std::shared_ptr<IndexBuffer>& VertexArray_OpenGL::GetIndexBuffer() const
	{
		return m_indexBuffer;
	}
}
