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
		case ShaderDataType::Float: { return sizeof(float) * 1; }
		case ShaderDataType::Float2: { return sizeof(float) * 2; }
		case ShaderDataType::Float3: { return sizeof(float) * 3; }
		case ShaderDataType::Float4: { return sizeof(float) * 4; }

		case ShaderDataType::Mat3: { return sizeof(float) * 3 * 3; }
		case ShaderDataType::Mat4: { return sizeof(float) * 4 * 4; }

		case ShaderDataType::Int: { return sizeof(int32_t) * 1; }
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
}