#ifndef NINJAPARTY_SPRITESHADER_HPP
#define NINJAPARTY_SPRITESHADER_HPP

#include <memory>
#include <stdint.h>
#include <string>

#include <NinjaParty/Math.hpp>

namespace NinjaParty
{
	enum class ShaderAttributes
	{
		Position = 0,
		Color,
		TexCoord,
		NumAttributes,
	};

    class FragmentShader;
    class Texture;
    class VertexShader;
    
	class SpriteShader
	{
	public:
        SpriteShader(VertexShader *vertexShader, FragmentShader *fragmentShader);
		~SpriteShader();
        
        void SetTexture(const std::string &parameterName, const Texture *texture, const int32_t &textureSlot);
        void SetTexture(const std::string &parameterName, const int32_t &textureId, const int32_t &textureSlot);

        void SetParameter(const std::string &parameterName, const int32_t &value);
        void SetParameter(const std::string &parameterName, const float &value);
        
        void SetParameter(const std::string &parameterName, const Vector2 &value);
        void SetParameter(const std::string &parameterName, const IntVector2 &value);
        void SetParameter(const std::string &parameterName, const Vector3 &value);
        void SetParameter(const std::string &parameterName, const Vector4 &value);
        
        void SetParameter(const std::string &parameterName, const Matrix2 &value);
        void SetParameter(const std::string &parameterName, const Matrix3 &value);
        void SetParameter(const std::string &parameterName, const Matrix4 &value);
        
        void Apply();

	protected:
        struct impl;
        std::unique_ptr<impl> pimpl;
	};
}

#endif//NINJAPARTY_SPRITESHADER_HPP
