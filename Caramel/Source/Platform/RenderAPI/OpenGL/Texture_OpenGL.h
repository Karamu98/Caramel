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

		virtual const std::string& GetPath() const override { return m_path; }

	private:
		void Load1D();
		void Load2D(const std::string& path);
		void Load3D();

		unsigned int m_ID;
		TextureProperties m_properties;
		std::string m_path;

		int m_textureType;
		int m_uWrapMode;
		int m_vWrapMode;
		int m_wWrapMode;

		int m_minFilterMode;
		int m_magFilterMode;
	};
}


