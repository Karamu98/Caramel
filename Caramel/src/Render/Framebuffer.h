#pragma once

namespace Caramel
{
	class Framebuffer
	{
		struct Data
		{
			unsigned int Width, Height;
		};

	public:
		Framebuffer(Framebuffer const&) = delete;
		void operator=(Framebuffer const&) = delete;
		Framebuffer(unsigned int width, unsigned int height);
		~Framebuffer();

		virtual const bool Bind() const = 0;
		virtual void ResizeFBO(unsigned int width, unsigned int height) = 0;

	private:
		unsigned int m_FBOhandle;
		unsigned int m_FBOWidth, m_FBOHeight;
	};
}