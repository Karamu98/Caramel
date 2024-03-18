#include "clpch.h"
#include "Texture_OpenGL.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Caramel
{
	GLenum TextureTypeToGLenum(const TextureType& type)
	{
		switch (type)
		{
		case TextureType::T_1D: return GL_TEXTURE_1D;
		case TextureType::T_2D: return GL_TEXTURE_2D;
		case TextureType::T_3D: return GL_TEXTURE_3D;
		case TextureType::T_Rectangle: return GL_TEXTURE_RECTANGLE;
		case TextureType::T_Buffer: return GL_TEXTURE_BUFFER;
		case TextureType::T_Cubemap: return GL_TEXTURE_CUBE_MAP;
		case TextureType::T_1DArray: return GL_TEXTURE_1D_ARRAY;
		case TextureType::T_2DArray: return GL_TEXTURE_2D_ARRAY;
		case TextureType::T_CubemapArray: return GL_TEXTURE_CUBE_MAP_ARRAY;
		case TextureType::T_2DMultisample: return GL_TEXTURE_2D_MULTISAMPLE;
		case TextureType::T_2DMultisampleArray: return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
		}

		return -1;
	}

	GLenum WrapModeToGLenum(const TextureWrapMode& wrapMode)
	{
		switch (wrapMode)
		{
		case TextureWrapMode::ClampToEdge: return GL_CLAMP_TO_EDGE;
		case TextureWrapMode::ClampToBorder: return GL_CLAMP_TO_BORDER;
		case TextureWrapMode::MirroredRepeat: return GL_MIRRORED_REPEAT;
		case TextureWrapMode::Repeat: return GL_REPEAT;
		case TextureWrapMode::MirrorClampToEdge: return GL_MIRROR_CLAMP_TO_EDGE;
		}

		return -1;
	}

	GLenum MinFilterToGLenum(const MinFilterMode& mode)
	{
		switch (mode)
		{
		case MinFilterMode::Nearest: return GL_NEAREST;
		case MinFilterMode::Linear: return GL_LINEAR;
		case MinFilterMode::NearestMipmap_Nearest: return GL_NEAREST_MIPMAP_NEAREST;
		case MinFilterMode::NearestMipmap_Linear: return GL_NEAREST_MIPMAP_LINEAR;
		case MinFilterMode::LinearMipmap_Nearest: return GL_LINEAR_MIPMAP_NEAREST;
		case MinFilterMode::LinearMipmap_Linear: return GL_LINEAR_MIPMAP_LINEAR;
		}

		return -1;
	}

	Texture_OpenGL::Texture_OpenGL(const TextureParams& params)
	{
		m_properties = params.Properties;

		m_textureType = TextureTypeToGLenum(m_properties.Type);
		m_uWrapMode = WrapModeToGLenum(m_properties.WrapMode_U);
		m_vWrapMode = WrapModeToGLenum(m_properties.WrapMode_V);
		m_wWrapMode = WrapModeToGLenum(m_properties.WrapMode_W);

		m_minFilterMode = MinFilterToGLenum(m_properties.MinFilter);
		m_magFilterMode = m_properties.MagFilter == MagFilterMode::Linear ? GL_LINEAR : GL_NEAREST;

		glGenTextures(1, &m_ID);
		glBindTexture(m_textureType, m_ID);

		if (m_properties.WrapMode_U != TextureWrapMode::None)
		{
			glTexParameteri(m_textureType, GL_TEXTURE_WRAP_S, m_uWrapMode);
		}
		if (m_properties.WrapMode_V != TextureWrapMode::None)
		{
			glTexParameteri(m_textureType, GL_TEXTURE_WRAP_T, m_vWrapMode);
		}
		if (m_properties.WrapMode_W != TextureWrapMode::None)
		{
			glTexParameteri(m_textureType, GL_TEXTURE_WRAP_R, m_wWrapMode);
		}

		glTexParameteri(m_textureType, GL_TEXTURE_MIN_FILTER, m_minFilterMode);
		glTexParameteri(m_textureType, GL_TEXTURE_MAG_FILTER, m_magFilterMode);

		// TODO: Change how we load the texture onto the GPU based on m_textureType (2D currently)
		{
			int width, height, nrChannels;
			unsigned char* data = stbi_load(params.Path.c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				GLenum format = GL_ZERO;
				switch (nrChannels)
				{
				case 1: format = GL_RED; break;
				case 3: format = GL_RGB; break;
				case 4: format = GL_RGBA; break;
				}

				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				CL_CORE_ERROR("Failed to load texture");
			}
			stbi_image_free(data);
		}

	}

	void Texture_OpenGL::Bind(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(m_textureType, m_ID);
	}

	void Texture_OpenGL::Unbind(unsigned int slot)
	{
		glBindTexture(m_textureType, 0);
	}
}

