#include "clpch.h"
#include "Render/Framebuffer.h"
#include <glad/glad.h>

#include "Core/Log.h"
#include "Render/Window.h"

namespace Caramel
{
	bool Framebuffer::Bind()
	{
		if (m_isValid)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			return true;
		}
		CL_CORE_ERROR("Framebuffer is not valid");
		return false;
	}

	void Framebuffer::ResizeFBO(unsigned int a_width, unsigned int a_height)
	{
		if (m_FBOWidth == a_width && m_FBOHeight == a_height)
		{
			return;
		}

		// If we're already valid, delete old resources
		if (m_isValid)
		{
			glDeleteFramebuffers(1, &m_FBO);
			glDeleteTextures(1, &m_colourTex);
			glDeleteRenderbuffers(1, &m_depthRender);
		}

		m_isValid = false;
		m_FBOWidth = a_width;
		m_FBOHeight = a_height;

		// Creating the FBO
		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		// Create a colour attachment texture
		glGenTextures(1, &m_colourTex);
		glBindTexture(GL_TEXTURE_2D, m_colourTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)m_FBOWidth, (GLsizei)m_FBOHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colourTex, 0);

		// Create a renderbuffer object for depth attachment (not a texture because we're not sampling)
		glGenRenderbuffers(1, &m_depthRender);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthRender);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, (GLsizei)m_FBOWidth, (GLsizei)m_FBOHeight); // Create the depth render buffer object
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRender); // Attach the render buffer object to the FBO depth slot

		// Test that the framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			CL_CORE_ERROR("Framebuffer is incomplete");
			return;
		}
		m_isValid = true;
		glViewport(0, 0, (GLsizei)m_FBOWidth, (GLsizei)m_FBOHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	Framebuffer::Framebuffer() : m_isValid(false)
	{
		ResizeFBO(AppWindow::GetWidth(), AppWindow::GetHeight());
	}

	Framebuffer::Framebuffer(unsigned int a_width, unsigned int a_height) : m_isValid(false)
	{
		ResizeFBO(a_width, a_height);
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &m_FBO);
		glDeleteTextures(1, &m_colourTex);
		glDeleteRenderbuffers(1, &m_depthRender);
	}

}

