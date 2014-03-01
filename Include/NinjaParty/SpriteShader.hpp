#ifndef NINJAPARTY_SPRITESHADER_HPP
#define NINJAPARTY_SPRITESHADER_HPP

#include <memory>
#include <stdint.h>

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

	class SpriteShader
	{
	public:
		SpriteShader();		
		~SpriteShader();

        void SetParameter(const std::string &parameterName, const int32_t &value);
        void SetParameter(const std::string &parameterName, const float &value);
        
        void SetParameter(const std::string &parameterName, const Vector2 &value);
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
