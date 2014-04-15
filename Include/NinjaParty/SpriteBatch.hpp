#ifndef NINJAPARTY_SPRITEBATCH_HPP
#define NINJAPARTY_SPRITEBATCH_HPP

#include <memory>
#include <string>

#include <NinjaParty/BlendMode.hpp>
#include <NinjaParty/Math.hpp>
#include <NinjaParty/Rectangle.hpp>

#include <NinjaParty/Color.hpp>
#include <NinjaParty/SpriteShader.hpp>
#include <NinjaParty/Texture.hpp>
#include <NinjaParty/TextureRegion.hpp>
#include <NinjaParty/Vertex.hpp>

namespace NinjaParty
{
	struct Font;
    
    class FragmentShader;
    class SpriteShader;
    class VertexShader;

	class SpriteBatch
	{
	public:
		// todo: spritebatch should default to figuring out the screen width/height
		SpriteBatch(int screenWidth, int screenHeight, int maxSpritesPerDraw = 1000);
		~SpriteBatch();

		void Begin(BlendMode blendMode = BlendMode::Alpha, const Matrix3 &batchTransform = Matrix3::IDENTITY, SpriteShader *spriteShader = nullptr, bool updateResolution = true);
		void Begin(BlendMode blendMode, const Matrix3 &batchTransform, const Matrix4 &projectionMatrix, SpriteShader *spriteShader, bool updateResolution);
		void End();

		void Draw(Texture *texture,
				  const Vector2 &position,
				  const Rectangle *sourceRectangle = nullptr,
				  const Vector2 &origin = Vector2(0, 0),
				  const float rotation = 0,
				  const Color &color = Color::White,
				  const Vector2 &scale = Vector2(1, 1),
				  const Matrix3 &transformMatrix = Matrix3::IDENTITY);

        void Draw(Texture *texture,
                  const TextureRegion &textureRegion,
                  const Vector2 &position,
                  const Vector2 &origin = Vector2::ZERO,
                  const float rotation = 0.0f,
                  const Color &color = Color::White,
                  const Vector2 &scale = Vector2::ONE,
                  const Matrix3 &transformMatrix = Matrix3::IDENTITY);
		
		void DrawNineSlice(Texture *texture,
						   const TextureRegion &textureRegion,
						   const Vector2 &position,
						   const Vector2 &dimensions,
						   const Vector2 &topLeftSlice,
						   const Vector2 &bottomRightSlice,
						   const Vector2 &origin = Vector2::ZERO,
						   const float rotation = 0,
						   const Color &color = Color::White);
		
		void DrawString(Font *font, Texture *texture, const std::string &s, const Vector2 &position, const Color &color);

		void DrawString(Font *font,
						Texture *texture,
						const std::string &s,
						const Vector2 &position,
						const Vector2 &origin = Vector2::ZERO,
						const float rotation = 0,
						const Color &color = Color::White,
						const Vector2 &scale = Vector2::ONE);

        const Matrix3& GetBatchTransform() const;
        void SetBatchTransform(const Matrix3 &transform);
        void ApplyTransform(const Matrix3 transform);
        
		void SetResolution(int screenWidth, int screenHeight);
        
        VertexShader* DefaultVertexShader() const;
        FragmentShader* DefaultFragmentShader() const;

	private:
        struct impl;
        std::unique_ptr<impl> pimpl;
	};
}

#endif//NINJAPARTY_SPRITEBATCH_HPP
