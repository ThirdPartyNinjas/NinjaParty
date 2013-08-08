#ifndef NINJAPARTY_SPRITEBATCH_HPP
#define NINJAPARTY_SPRITEBATCH_HPP

#include <string>

#include <NinjaParty/Matrix3.hpp>
#include <NinjaParty/Rectangle.hpp>
#include <NinjaParty/Vector2.hpp>

#include <NinjaParty/Color.hpp>
#include <NinjaParty/SpriteShader.hpp>
#include <NinjaParty/Texture.hpp>
#include <NinjaParty/TextureRegion.hpp>
#include <NinjaParty/Vertex.hpp>

namespace NinjaParty
{
	struct Font;

	enum class BlendMode
	{
		None = 0,
		Alpha,
		PremultipliedAlpha,
		Additive,
	};

	class SpriteBatch
	{
	public:
		// todo: spritebatch should default to figuring out the screen width/height
		SpriteBatch(int screenWidth, int screenHeight, int maxSpritesPerDraw = 1000);
		~SpriteBatch();

		void Begin(BlendMode blendMode = BlendMode::Alpha, const Matrix3 &batchTransform = Matrix3::IDENTITY, bool updateResolution = true);
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

        const Matrix3& GetBatchTransform() const { return batchTransform; }
        void SetBatchTransform(const Matrix3 &transform) { batchTransform = transform; }
        void ApplyTransform(const Matrix3 transform) { batchTransform = transform * batchTransform; }
        
		void SetResolution(int screenWidth, int screenHeight);

	private:
		void DrawBuffer();
		
		unsigned int currentTextureId;
		SpriteShader *currentShader;
		SpriteShader defaultShader;
		
		int screenWidth, screenHeight;
		
		BlendMode blendMode;
		
		Vertex *vertices;
		int maxVertices;
		int activeVertices;
		
		Matrix3 batchTransform;
	};
}

#endif//NINJAPARTY_SPRITEBATCH_HPP
