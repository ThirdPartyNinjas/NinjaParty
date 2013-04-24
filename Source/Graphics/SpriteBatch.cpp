#include <NinjaParty/Font.hpp>
#include <NinjaParty/MathHelpers.hpp>
#include <NinjaParty/SpriteBatch.hpp>
#include <NinjaParty/Vector3.hpp>

namespace NinjaParty
{
	SpriteBatch::SpriteBatch(int screenWidth, int screenHeight, int maxSpritesPerDraw)
	{
		currentShader = &defaultShader;
		SetResolution(screenWidth, screenHeight);
		
		currentTextureId = 0;
		
		maxVertices = maxSpritesPerDraw * 6;
		activeVertices = 0;
		vertices = new Vertex[maxVertices];
	}
	
	SpriteBatch::~SpriteBatch()
	{
		delete[] vertices;
	}
	
	void SpriteBatch::Begin(BlendMode blendMode, const Matrix3 &batchTransform, bool updateResolution)
	{
		if(updateResolution)
		{
			int viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);
			SetResolution(viewport[2], viewport[3]);
		}
		
		this->blendMode = blendMode;
		this->batchTransform = batchTransform;
		currentTextureId = 0;
		activeVertices = 0;
	}
	
	void SpriteBatch::End()
	{
		if(activeVertices < 6)
			return;
		
		if(activeVertices >= 0)
			DrawBuffer();
		
		currentTextureId = 0;
		activeVertices = 0;
	}
	
	void SpriteBatch::SetResolution(int screenWidth, int screenHeight)
	{
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		
		currentShader->SetOrthoMatrix(screenWidth, screenHeight);
	}
	
	void SpriteBatch::Draw(Texture *texture, const Vector2 &position, const Rectangle *sourceRectangle,
						   const Vector2 &origin, const float rotation, const Color &color, const Vector2 &scale, const Matrix3 &transformMatrix)
	{
		if(activeVertices == maxVertices || (currentTextureId != 0 && currentTextureId != texture->GetTextureId()))
			DrawBuffer();
		
		currentTextureId = texture->GetTextureId();
		
		Rectangle textureRect(0, 0, texture->GetWidth(), texture->GetHeight());
		
		if(sourceRectangle == nullptr)
			sourceRectangle = &textureRect;
		
		int width = sourceRectangle->width;
		int height = sourceRectangle->height;
		
		int textureWidth = texture->GetWidth();
		int textureHeight = texture->GetHeight();
		
		Vector3 v;
		Matrix3 transform = batchTransform * transformMatrix * CreateTranslationMatrix(position.X(), position.Y()) *
            CreateRotationMatrix(rotation) * CreateScaleMatrix(scale.X(), scale.Y());
		
		v.X() = -width * origin.X();
		v.Y() = -height * origin.Y();
		v.Z() = 1;
		v = transform * v;
		
		vertices[activeVertices].x = v.X();
		vertices[activeVertices].y = v.Y();
		vertices[activeVertices].r = color.R();
		vertices[activeVertices].g = color.G();
		vertices[activeVertices].b = color.B();
		vertices[activeVertices].a = color.A();
		vertices[activeVertices].u = (float)sourceRectangle->x / textureWidth;
		vertices[activeVertices++].v = (float)sourceRectangle->y / textureHeight;
		
		v.X() = width * (1 - origin.X());
		v.Y() = -height * origin.Y();
		v = transform * v;
		
		vertices[activeVertices].x = v.X();
		vertices[activeVertices].y = v.Y();
		vertices[activeVertices].r = color.R();
		vertices[activeVertices].g = color.G();
		vertices[activeVertices].b = color.B();
		vertices[activeVertices].a = color.A();
		vertices[activeVertices].u = (float)(sourceRectangle->x + sourceRectangle->width) / textureWidth;
		vertices[activeVertices++].v = (float)sourceRectangle->y / textureHeight;
		
		v.X() = -width * origin.X();
		v.Y() = height * (1 - origin.Y());
		v = transform * v;
		
		vertices[activeVertices].x = v.X();
		vertices[activeVertices].y = v.Y();
		vertices[activeVertices].r = color.R();
		vertices[activeVertices].g = color.G();
		vertices[activeVertices].b = color.B();
		vertices[activeVertices].a = color.A();
		vertices[activeVertices].u = (float)sourceRectangle->x / textureWidth;
		vertices[activeVertices++].v = (float)(sourceRectangle->y + sourceRectangle->height) / textureHeight;
		
		vertices[activeVertices] = vertices[activeVertices - 1];
		activeVertices++;
		vertices[activeVertices] = vertices[activeVertices - 3];
		activeVertices++;
		
		v.X() = width * (1 - origin.X());
		v.Y() = height * (1 - origin.Y());
		v = transform * v;
		
		vertices[activeVertices].x = v.X();
		vertices[activeVertices].y = v.Y();
		vertices[activeVertices].r = color.R();
		vertices[activeVertices].g = color.G();
		vertices[activeVertices].b = color.B();
		vertices[activeVertices].a = color.A();
		vertices[activeVertices].u = (float)(sourceRectangle->x + sourceRectangle->width) / textureWidth;
		vertices[activeVertices++].v = (float)(sourceRectangle->y + sourceRectangle->height) / textureHeight;
	}
	
    void SpriteBatch::Draw(Texture *texture,
						   const TextureRegion &textureRegion,
						   const Vector2 &position,
						   const Vector2 &originInput,
						   const float rotation,
						   const Color &color,
						   const Vector2 &scale,
						   const Matrix3 &transformMatrix)
	{
		if(activeVertices == maxVertices || (currentTextureId != 0 && currentTextureId != texture->GetTextureId()))
			DrawBuffer();
		
		currentTextureId = texture->GetTextureId();
		
		const Rectangle *sourceRectangle = &textureRegion.bounds;
		Vector2 origin;
		
		origin.X() = Lerp(textureRegion.originTopLeft.X(), textureRegion.originBottomRight.X(), originInput.X());
		origin.Y() = Lerp(textureRegion.originTopLeft.Y(), textureRegion.originBottomRight.Y(), originInput.Y());
		
		int width = sourceRectangle->width;
		int height = sourceRectangle->height;
		
		int textureWidth = texture->GetWidth();
		int textureHeight = texture->GetHeight();
		
		Vector3 v;
		Matrix3 transform = batchTransform * transformMatrix * CreateTranslationMatrix(position.X(), position.Y()) *
		CreateRotationMatrix(rotation) * CreateScaleMatrix(scale.X(), scale.Y());
		
		v.X() = -width * origin.X();
		v.Y() = -height * origin.Y();
		v.Z() = 1;
		v = transform * v;
		
		vertices[activeVertices].x = v.X();
		vertices[activeVertices].y = v.Y();
		vertices[activeVertices].r = color.R();
		vertices[activeVertices].g = color.G();
		vertices[activeVertices].b = color.B();
		vertices[activeVertices].a = color.A();
		if(textureRegion.rotated)
		{
			vertices[activeVertices].u = (float)(sourceRectangle->x + sourceRectangle->height) / textureWidth;
			vertices[activeVertices++].v = (float)sourceRectangle->y / textureHeight;
		}
		else
		{
			vertices[activeVertices].u = (float)sourceRectangle->x / textureWidth;
			vertices[activeVertices++].v = (float)sourceRectangle->y / textureHeight;
		}
		
		v.X() = width * (1 - origin.X());
		v.Y() = -height * origin.Y();
		v = transform * v;
		
		vertices[activeVertices].x = v.X();
		vertices[activeVertices].y = v.Y();
		vertices[activeVertices].r = color.R();
		vertices[activeVertices].g = color.G();
		vertices[activeVertices].b = color.B();
		vertices[activeVertices].a = color.A();
		if(textureRegion.rotated)
		{
			vertices[activeVertices].u = (float)(sourceRectangle->x + sourceRectangle->height) / textureWidth;
			vertices[activeVertices++].v = (float)(sourceRectangle->y + sourceRectangle->width) / textureHeight;
		}
		else
		{
			vertices[activeVertices].u = (float)(sourceRectangle->x + sourceRectangle->width) / textureWidth;
			vertices[activeVertices++].v = (float)sourceRectangle->y / textureHeight;
		}
		
		v.X() = -width * origin.X();
		v.Y() = height * (1 - origin.Y());
		v = transform * v;
		
		vertices[activeVertices].x = v.X();
		vertices[activeVertices].y = v.Y();
		vertices[activeVertices].r = color.R();
		vertices[activeVertices].g = color.G();
		vertices[activeVertices].b = color.B();
		vertices[activeVertices].a = color.A();
		if(textureRegion.rotated)
		{
			vertices[activeVertices].u = (float)sourceRectangle->x / textureWidth;
			vertices[activeVertices++].v = (float)sourceRectangle->y / textureHeight;
		}
		else
		{
			vertices[activeVertices].u = (float)sourceRectangle->x / textureWidth;
			vertices[activeVertices++].v = (float)(sourceRectangle->y + sourceRectangle->height) / textureHeight;
		}
		
		vertices[activeVertices] = vertices[activeVertices - 1];
		activeVertices++;
		vertices[activeVertices] = vertices[activeVertices - 3];
		activeVertices++;
		
		v.X() = width * (1 - origin.X());
		v.Y() = height * (1 - origin.Y());
		v = transform * v;
		
		vertices[activeVertices].x = v.X();
		vertices[activeVertices].y = v.Y();
		vertices[activeVertices].r = color.R();
		vertices[activeVertices].g = color.G();
		vertices[activeVertices].b = color.B();
		vertices[activeVertices].a = color.A();
		if(textureRegion.rotated)
		{
			vertices[activeVertices].u = (float)sourceRectangle->x / textureWidth;
			vertices[activeVertices++].v = (float)(sourceRectangle->y + sourceRectangle->width) / textureHeight;
		}
		else
		{
			vertices[activeVertices].u = (float)(sourceRectangle->x + sourceRectangle->width) / textureWidth;
			vertices[activeVertices++].v = (float)(sourceRectangle->y + sourceRectangle->height) / textureHeight;
		}		
	}
    

	void SpriteBatch::DrawString(Font *font, Texture *texture, const std::string &s, const Vector2 &position, const Color &color)
	{
		if(activeVertices == maxVertices || (currentTextureId != 0 && currentTextureId != texture->GetTextureId()))
			DrawBuffer();
		
		currentTextureId = texture->GetTextureId();
		
		float x = 0, y = 0;
		
		for(size_t i=0; i<s.size(); i++)
		{
			if(s[i] == '\n')
			{
				y += font->height;
				continue;
			}
			if(s[i] == '\r')
			{
				x = 0;
				continue;
			}
			
			CharacterData &cd = (font->characters.count(s[i]) == 1) ? font->characters[s[i]] : font->characters[' '];
			
			if(cd.width == 0 || cd.height == 0)
			{
				x += cd.advanceX;
				continue;
			}
			
			if(activeVertices == maxVertices)
				DrawBuffer();
			
			const Vector2 &origin = Vector2::ZERO;
			
			int width = cd.width;
			int height = cd.height;
			
			int textureWidth = texture->GetWidth();
			int textureHeight = texture->GetHeight();
			
			Vector3 v;
			Matrix3 transform = batchTransform * CreateTranslationMatrix(position.X() + x + cd.offsetX, position.Y() + y + cd.offsetY);
			
			v.X() = -width * origin.X();
			v.Y() = -height * origin.Y();
			v.Z() = 1;
			v = transform * v;
			
			vertices[activeVertices].x = v.X();
			vertices[activeVertices].y = v.Y();
			vertices[activeVertices].r = color.R();
			vertices[activeVertices].g = color.G();
			vertices[activeVertices].b = color.B();
			vertices[activeVertices].a = color.A();
			vertices[activeVertices].u = (float)cd.x / textureWidth;
			vertices[activeVertices++].v = (float)cd.y / textureHeight;
			
			v.X() = width * (1 - origin.X());
			v.Y() = -height * origin.Y();
			v = transform * v;
			
			vertices[activeVertices].x = v.X();
			vertices[activeVertices].y = v.Y();
			vertices[activeVertices].r = color.R();
			vertices[activeVertices].g = color.G();
			vertices[activeVertices].b = color.B();
			vertices[activeVertices].a = color.A();
			vertices[activeVertices].u = (float)(cd.x + cd.width) / textureWidth;
			vertices[activeVertices++].v = (float)cd.y / textureHeight;
			
			v.X() = -width * origin.X();
			v.Y() = height * (1 - origin.Y());
			v = transform * v;
			
			vertices[activeVertices].x = v.X();
			vertices[activeVertices].y = v.Y();
			vertices[activeVertices].r = color.R();
			vertices[activeVertices].g = color.G();
			vertices[activeVertices].b = color.B();
			vertices[activeVertices].a = color.A();
			vertices[activeVertices].u = (float)cd.x / textureWidth;
			vertices[activeVertices++].v = (float)(cd.y + cd.height) / textureHeight;
			
			vertices[activeVertices] = vertices[activeVertices - 1];
			activeVertices++;
			vertices[activeVertices] = vertices[activeVertices - 3];
			activeVertices++;
			
			v.X() = width * (1 - origin.X());
			v.Y() = height * (1 - origin.Y());
			v = transform * v;
			
			vertices[activeVertices].x = v.X();
			vertices[activeVertices].y = v.Y();
			vertices[activeVertices].r = color.R();
			vertices[activeVertices].g = color.G();
			vertices[activeVertices].b = color.B();
			vertices[activeVertices].a = color.A();
			vertices[activeVertices].u = (float)(cd.x + cd.width) / textureWidth;
			vertices[activeVertices++].v = (float)(cd.y + cd.height) / textureHeight;
			
			x += cd.advanceX;
		}
	}
	
	void SpriteBatch::DrawString(Font *font,
					Texture *texture,
					const std::string &s,
					const Vector2 &position,
					const Vector2 &origin,
					const float rotation,
					const Color &color,
					const Vector2 &scale)
	{
		NinjaParty::Vector2 maximum;
		
		MeasureString(font, s, maximum);
		NinjaParty::Vector2 o;
		o.X() = maximum.X() * origin.X();
		o.Y() = maximum.Y() * origin.Y();
		
		if(activeVertices == maxVertices || (currentTextureId != 0 && currentTextureId != texture->GetTextureId()))
			DrawBuffer();
		
		currentTextureId = texture->GetTextureId();
		
		float x = 0, y = 0;
		
		for(size_t i=0; i<s.size(); i++)
		{
			if(s[i] == '\n')
			{
				y += font->height;
				continue;
			}
			if(s[i] == '\r')
			{
				x = 0;
				continue;
			}
			
			CharacterData &cd = (font->characters.count(s[i]) == 1) ? font->characters[s[i]] : font->characters[' '];
			
			if(cd.width == 0 || cd.height == 0)
			{
				x += cd.advanceX;
				continue;
			}
			
			if(activeVertices == maxVertices)
				DrawBuffer();
			
			int width = cd.width;
			int height = cd.height;
			
			int textureWidth = texture->GetWidth();
			int textureHeight = texture->GetHeight();
			
			Vector3 v;			
			Matrix3 transform =
				CreateTranslationMatrix(position.X(), position.Y())
				* CreateRotationMatrix(rotation)
				* CreateScaleMatrix(scale.X(), scale.Y())
				* CreateTranslationMatrix(-o.X(), -o.Y())
				* CreateTranslationMatrix(x + cd.offsetX, y + cd.offsetY);
			
			v.X() = 0;
			v.Y() = 0;
			v.Z() = 1;
			v = transform * v;
			
			vertices[activeVertices].x = v.X();
			vertices[activeVertices].y = v.Y();
			vertices[activeVertices].r = color.R();
			vertices[activeVertices].g = color.G();
			vertices[activeVertices].b = color.B();
			vertices[activeVertices].a = color.A();
			vertices[activeVertices].u = (float)cd.x / textureWidth;
			vertices[activeVertices++].v = (float)cd.y / textureHeight;
			
			v.X() = width;
			v.Y() = 0;
			v = transform * v;
			
			vertices[activeVertices].x = v.X();
			vertices[activeVertices].y = v.Y();
			vertices[activeVertices].r = color.R();
			vertices[activeVertices].g = color.G();
			vertices[activeVertices].b = color.B();
			vertices[activeVertices].a = color.A();
			vertices[activeVertices].u = (float)(cd.x + cd.width) / textureWidth;
			vertices[activeVertices++].v = (float)cd.y / textureHeight;
			
			v.X() = 0;
			v.Y() = height;
			v = transform * v;
			
			vertices[activeVertices].x = v.X();
			vertices[activeVertices].y = v.Y();
			vertices[activeVertices].r = color.R();
			vertices[activeVertices].g = color.G();
			vertices[activeVertices].b = color.B();
			vertices[activeVertices].a = color.A();
			vertices[activeVertices].u = (float)cd.x / textureWidth;
			vertices[activeVertices++].v = (float)(cd.y + cd.height) / textureHeight;
			
			vertices[activeVertices] = vertices[activeVertices - 1];
			activeVertices++;
			vertices[activeVertices] = vertices[activeVertices - 3];
			activeVertices++;
			
			v.X() = width;
			v.Y() = height;
			v = transform * v;
			
			vertices[activeVertices].x = v.X();
			vertices[activeVertices].y = v.Y();
			vertices[activeVertices].r = color.R();
			vertices[activeVertices].g = color.G();
			vertices[activeVertices].b = color.B();
			vertices[activeVertices].a = color.A();
			vertices[activeVertices].u = (float)(cd.x + cd.width) / textureWidth;
			vertices[activeVertices++].v = (float)(cd.y + cd.height) / textureHeight;
			
			x += cd.advanceX;
		}
	}

	void SpriteBatch::DrawBuffer()
	{
		switch(blendMode)
		{
			case BlendMode::None:
				glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
				glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ONE, GL_ZERO);
				break;
			case BlendMode::Alpha:
				glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
				glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
				break;
			case BlendMode::Additive:
				glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
				glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ZERO);
				break;
			case BlendMode::PremultipliedAlpha:
				glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
				glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
				break;
		}
		
		currentShader->Draw(vertices, activeVertices, currentTextureId);
		
		activeVertices = 0;
	}
}
