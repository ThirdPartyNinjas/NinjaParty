#ifndef NINJAPARTY_SPRITESHADER_HPP
#define NINJAPARTY_SPRITESHADER_HPP

#include <string>

#include <NinjaParty/IncludeGL.h>

namespace NinjaParty
{
	struct Vertex;
	class Texture;

	enum class ShaderAttributes
	{
		Position = 0,
		Color,
		TexCoord,
		NumAttributes,
	};

	enum class ShaderUniforms
	{
		Projection = 0,
		Sampler,
		NumUniforms,
	};
	
	class SpriteShader
	{
	public:
		SpriteShader();
		SpriteShader(const std::string &vertexShaderFile, const std::string &fragmentShaderFile);
		
		~SpriteShader();

		void Draw(Vertex *vertices, int vertexCount, GLuint textureId);

		void SetProjectionMatrix(GLfloat matrix[16]) { for(int i=0; i<16; i++) projectionMatrix[i] = matrix[i]; }

		void SetOrthoMatrix(int width, int height);

	protected:
		unsigned int shaderId;

		GLfloat projectionMatrix[16];
		int uniforms[(int)ShaderUniforms::NumUniforms];
	};
}

#endif//NINJAPARTY_SPRITESHADER_HPP
