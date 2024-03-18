#pragma once

namespace Caramel
{
	enum class TextureType
	{
		None,
		T_1D,
		T_2D,
		T_3D,
		T_Rectangle,
		T_Buffer,
		T_Cubemap,
		T_1DArray,
		T_2DArray,
		T_CubemapArray,
		T_2DMultisample,
		T_2DMultisampleArray,
	};

	enum class TextureWrapMode
	{
		None,
		ClampToEdge,
		ClampToBorder,
		MirroredRepeat,
		Repeat,
		MirrorClampToEdge
	};

	enum class MinFilterMode
	{
		Nearest,
		Linear,
		NearestMipmap_Nearest,
		NearestMipmap_Linear,
		LinearMipmap_Nearest,
		LinearMipmap_Linear
	};

	enum class MagFilterMode
	{
		Nearest,
		Linear
	};

	struct TextureProperties
	{
		TextureType Type = TextureType::T_1D;
		MinFilterMode MinFilter;
		MagFilterMode MagFilter;
		TextureWrapMode WrapMode_U = TextureWrapMode::ClampToEdge;
		TextureWrapMode WrapMode_V;
		TextureWrapMode WrapMode_W;
	};

	struct TextureParams
	{
		const std::string& Path;
		const TextureProperties& Properties;
	};

	class Texture
	{
	public:
		virtual ~Texture() {}

		virtual void Bind(unsigned int slot = 0) = 0;
		virtual void Unbind(unsigned int slot = 0) = 0;

		virtual const TextureProperties& GetProperties() const = 0;


		static std::shared_ptr<Texture> Create(const TextureParams& params);
	};
}


