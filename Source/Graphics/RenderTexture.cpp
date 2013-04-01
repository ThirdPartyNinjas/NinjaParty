#include <stdexcept>

#include <NinjaParty/IncludeGL.h>
#include <NinjaParty/RenderTexture.hpp>

namespace NinjaParty
{
	RenderTexture::RenderTexture(int width, int height)
	{
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo);
		
		this->width = width;
		this->height = height;
		
		glGenTextures(1, &textureId);
		
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		
		glGenFramebuffers(1, &frameBufferObject);
 		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
	
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(status != GL_FRAMEBUFFER_COMPLETE)
		{
			throw std::runtime_error("Failed to create RenderTexture");
		}
		
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	}
	
	RenderTexture::~RenderTexture()
	{
		glDeleteFramebuffers(1, &frameBufferObject);
	}
	
	void RenderTexture::Bind()
	{
		glGetIntegerv(GL_VIEWPORT, viewport);
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo);
		
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
		
		glViewport(0, 0, width, height);
		
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	
	void RenderTexture::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	}
}
