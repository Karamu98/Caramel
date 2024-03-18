#pragma once

#include "Core/RenderAPI/Texture.h"

namespace Caramel
{
	class Texture_OpenGL : public Texture
	{
	public:
		Texture_OpenGL(const TextureParams& params);

		void Bind(unsigned int slot) override;
		void Unbind(unsigned int slot) override;
		virtual const TextureProperties& GetProperties() const override { return m_properties; }

	private:
		unsigned int m_ID;
		TextureProperties m_properties;

		int m_textureType;
		int m_uWrapMode;
		int m_vWrapMode;
		int m_wWrapMode;

		int m_minFilterMode;
		int m_magFilterMode;
	};
}


