#ifndef NINJAPARTY_RENDERTEXTURE_HPP
#define NINJAPARTY_RENDERTEXTURE_HPP

#include <NinjaParty/Texture.hpp>

namespace NinjaParty
{
	class RenderTexture : public Texture
	{
	public:
		RenderTexture(int width, int height);
		~RenderTexture();
		
		void Bind();
		void Unbind();
		
		virtual void Invalidate() { Texture::Invalidate(); frameBufferObject = 0; }
	private:
		unsigned int frameBufferObject;
		int fbo;
		
		int viewport[4];
	};
}

#endif//NINJAPARTY_RENDERTEXTURE_HPP
