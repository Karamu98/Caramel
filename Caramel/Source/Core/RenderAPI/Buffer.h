#pragma once


namespace Caramel
{
	enum class ShaderDataType : uint8_t
	{
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,

		Mat3,
		Mat4,

		Int,
		Int2,
		Int3,
		Int4,

		Bool
	};

	static const inline uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		if (type == ShaderDataType::None)
		{
			return -1;
		}

		switch (type)
		{
		case ShaderDataType::Float:  { return sizeof(float)  * 1; }
		case ShaderDataType::Float2: { return sizeof(float) * 2; }
		case ShaderDataType::Float3: { return sizeof(float) * 3; }
		case ShaderDataType::Float4: { return sizeof(float) * 4; }

		case ShaderDataType::Mat3: { return sizeof(float) * 3 * 3; }
		case ShaderDataType::Mat4: { return sizeof(float) * 4 * 4; }

		case ShaderDataType::Int:  { return sizeof(int32_t) * 1; }
		case ShaderDataType::Int2: { return sizeof(int32_t) * 2; }
		case ShaderDataType::Int3: { return sizeof(int32_t) * 3; }
		case ShaderDataType::Int4: { return sizeof(int32_t) * 4; }

		case ShaderDataType::Bool: { return sizeof(bool); }
		}
	}

	static const inline uint32_t GetTypeComponentCount(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:
		case ShaderDataType::Int:
		case ShaderDataType::Bool:
		{
			return 1;
		}

		case ShaderDataType::Float2:
		case ShaderDataType::Int2:
		{
			return 2;
		}

		case ShaderDataType::Float3:
		case ShaderDataType::Int3:
		{
			return 3;
		}

		case ShaderDataType::Float4:
		case ShaderDataType::Int4:
		{
			return 4;
		}

		case ShaderDataType::Mat3:
		{
			return 3 * 3;
		}
		case ShaderDataType::Mat4:
		{
			return 4 * 4;
		}
		}
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		bool Normalised;

		BufferElement() {}; 
		BufferElement(ShaderDataType type, const std::string& name) : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalised(false) {}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& initList) : m_elements(initList) { CalcOffsetAndStride(); }
		std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_elements.end(); }

		std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }

		const inline uint32_t GetStride() const { return m_stride; }
		const inline std::vector<BufferElement>& GetElements() const { return m_elements; };

	private:
		void CalcOffsetAndStride()
		{
			uint32_t offset = 0;
			m_stride = 0;
			
			for (auto& ele : m_elements)
			{
				ele.Offset = offset;
				offset += ele.Size;
			}

			m_stride = offset;
		}

		std::vector<BufferElement> m_elements;
		uint32_t m_stride;
	};


	struct VertexBufferParams
	{
		float* Verticies;
		uint32_t Size;
	};

	struct IndexBufferParams
	{
		uint32_t* Indicies;
		uint32_t Size;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() = 0;

		static std::shared_ptr<VertexBuffer> Create(const VertexBufferParams& params);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static std::shared_ptr<IndexBuffer> Create(const IndexBufferParams& params);
	};


	class VertexArray
	{
	public:
		virtual ~VertexArray() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;


		static std::shared_ptr<VertexArray> Create();
	};
}


