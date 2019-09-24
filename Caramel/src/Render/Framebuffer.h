#pragma once

namespace Caramel
{
	class Framebuffer
	{
	public:
		Framebuffer();
		Framebuffer(unsigned int a_width, unsigned int a_height);
		~Framebuffer();
		bool Bind();

		// TEMP
		void ResizeFBO(unsigned int a_width, unsigned int a_height);

	public:
		unsigned int m_FBO;
		unsigned int m_colourTex;
		unsigned int m_depthRender;
		unsigned int m_FBOWidth, m_FBOHeight;
		bool m_isValid;
	};
}