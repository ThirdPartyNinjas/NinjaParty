#include <stdexcept>

#include <NinjaParty/Font.hpp>
#include <NinjaParty/IncludeGL.h>
#include <NinjaParty/MathHelpers.hpp>
#include <NinjaParty/SpriteBatch.hpp>
#include <NinjaParty/Math.hpp>

namespace NinjaParty
{
    struct SpriteBatch::impl
    {
        Matrix4 defaultProjectionMatrix;
        Matrix4 currentProjectionMatrix;
        
		unsigned int currentTextureId;
		SpriteShader *currentShader;
		SpriteShader defaultShader;
		
		int screenWidth, screenHeight;
		
		BlendMode blendMode;
		
        // todo: unique_ptr this
		Vertex *vertices;
		int maxVertices;
		int activeVertices;
		
		Matrix3 batchTransform;
        
		void DrawBuffer();
    };
    
	SpriteBatch::SpriteBatch(int screenWidth, int screenHeight, int maxSpritesPerDraw)
    : pimpl(new impl)
	{
		pimpl->currentShader = &pimpl->defaultShader;
		SetResolution(screenWidth, screenHeight);
		
		pimpl->currentTextureId = 0;
        
		pimpl->maxVertices = maxSpritesPerDraw * 6;
		pimpl->activeVertices = 0;
		pimpl->vertices = new Vertex[pimpl->maxVertices];
	}
	
	SpriteBatch::~SpriteBatch()
	{
		delete[] pimpl->vertices;
	}
    
    void SpriteBatch::Begin(BlendMode blendMode, const Matrix3 &batchTransform, bool updateResolution)
    {
        Begin(blendMode, batchTransform, pimpl->defaultProjectionMatrix, updateResolution);
    }
	
	void SpriteBatch::Begin(BlendMode blendMode, const Matrix3 &batchTransform, const Matrix4 &projectionMatrix, bool updateResolution)
	{
		if(updateResolution)
		{
			int viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);
			SetResolution(viewport[2], viewport[3]);
		}
		
		pimpl->blendMode = blendMode;
		pimpl->batchTransform = batchTransform;
		pimpl->currentTextureId = 0;
		pimpl->activeVertices = 0;
        
        pimpl->currentProjectionMatrix = projectionMatrix;
        
        pimpl->currentShader->Apply();
	}
	
	void SpriteBatch::End()
	{
		if(pimpl->activeVertices < 3)
			return;
		
        pimpl->DrawBuffer();
		
		pimpl->currentTextureId = 0;
		pimpl->activeVertices = 0;
	}
	
	void SpriteBatch::SetResolution(int screenWidth, int screenHeight)
	{
		pimpl->screenWidth = screenWidth;
		pimpl->screenHeight = screenHeight;
		
        pimpl->defaultProjectionMatrix = CreateOrthographicProjectionMatrix(0, screenWidth, 0, screenHeight, 1, -1);
	}
	
	void SpriteBatch::Draw(Texture *texture, const Vector2 &position, const Rectangle *sourceRectangle,
						   const Vector2 &origin, const float rotation, const Color &color, const Vector2 &scale, const Matrix3 &transformMatrix)
	{
		if(pimpl->activeVertices == pimpl->maxVertices || (pimpl->currentTextureId != 0 && pimpl->currentTextureId != texture->GetTextureId()))
			pimpl->DrawBuffer();
		
		pimpl->currentTextureId = texture->GetTextureId();
		
		Rectangle textureRect(0, 0, texture->GetWidth(), texture->GetHeight());
		
		if(sourceRectangle == nullptr)
			sourceRectangle = &textureRect;
		
		int width = sourceRectangle->width;
		int height = sourceRectangle->height;
		
		int textureWidth = texture->GetWidth();
		int textureHeight = texture->GetHeight();
		
		Vector3 v;
		Matrix3 transform = pimpl->batchTransform * transformMatrix * CreateTranslationMatrix(position.X(), position.Y()) *
            CreateRotationMatrix(rotation) * CreateScaleMatrix(scale.X(), scale.Y());
		
		v.X() = -width * origin.X();
		v.Y() = -height * origin.Y();
		v.Z() = 1;
		v = transform * v;
        
        Vertex *vertices = pimpl->vertices;
        int &activeVertices = pimpl->activeVertices;
		
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
		if(pimpl->activeVertices == pimpl->maxVertices || (pimpl->currentTextureId != 0 && pimpl->currentTextureId != texture->GetTextureId()))
			pimpl->DrawBuffer();
		
		pimpl->currentTextureId = texture->GetTextureId();
		
		const Rectangle *sourceRectangle = &textureRegion.bounds;
		Vector2 origin;
		
		origin.X() = Lerp(textureRegion.originTopLeft.X(), textureRegion.originBottomRight.X(), originInput.X());
		origin.Y() = Lerp(textureRegion.originTopLeft.Y(), textureRegion.originBottomRight.Y(), originInput.Y());
		
		int width = sourceRectangle->width;
		int height = sourceRectangle->height;
		
		int textureWidth = texture->GetWidth();
		int textureHeight = texture->GetHeight();
		
		Vector3 v;
		Matrix3 transform = pimpl->batchTransform * transformMatrix * CreateTranslationMatrix(position.X(), position.Y()) *
		CreateRotationMatrix(rotation) * CreateScaleMatrix(scale.X(), scale.Y());
		
		v.X() = -width * origin.X();
		v.Y() = -height * origin.Y();
		v.Z() = 1;
		v = transform * v;
		
        Vertex *vertices = pimpl->vertices;
        int &activeVertices = pimpl->activeVertices;

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
    
	void SpriteBatch::DrawNineSlice(Texture *texture,
									const TextureRegion &textureRegion,
									const Vector2 &position,
									const Vector2 &dimensions,
									const Vector2 &topLeftSlice,
									const Vector2 &bottomRightSlice,
									const Vector2 &origin,
									const float rotation,
									const Color &color)
	{
		if(textureRegion.rotated)
		{
			throw std::runtime_error("Nine Slice doesn't support rotated texture atlases yet");
		}
		else
		{
			Matrix3 transform = CreateTranslationMatrix(position.X(), position.Y()) *
				CreateRotationMatrix(rotation) *
				CreateTranslationMatrix(-origin.X() * dimensions.X(), -origin.Y() * dimensions.Y());
			
			float xScale = (dimensions.X() - topLeftSlice.X() - bottomRightSlice.X()) /
				(float)(textureRegion.bounds.width - topLeftSlice.X() - bottomRightSlice.X());
			float yScale = (dimensions.Y() - topLeftSlice.Y() - bottomRightSlice.Y()) /
				(float)(textureRegion.bounds.height - topLeftSlice.Y() - bottomRightSlice.Y());
			
			float xMid = textureRegion.bounds.width - topLeftSlice.X() - bottomRightSlice.X();
			float yMid = textureRegion.bounds.height - topLeftSlice.Y() - bottomRightSlice.Y();
			float xMidScaled = dimensions.X() - topLeftSlice.X() - bottomRightSlice.X();
			float yMidScaled = dimensions.Y() - topLeftSlice.Y() - bottomRightSlice.Y();
			
			
			// top left
			Rectangle rectangle(textureRegion.bounds.x,
								textureRegion.bounds.y,
								topLeftSlice.X(),
								topLeftSlice.Y());
			
			Draw(texture,
				 Vector2::ZERO,
				 &rectangle,
				 Vector2::ZERO,
				 0,
				 color,
				 Vector2(1.0f, 1.0f),
				 transform);

			// top mid
			rectangle = Rectangle(textureRegion.bounds.x + topLeftSlice.X(),
								  textureRegion.bounds.y,
								  xMid,
								  topLeftSlice.Y());
			
			Draw(texture,
				 Vector2(topLeftSlice.X(), 0),
				 &rectangle,
				 Vector2::ZERO,
				 0,
				 color,
				 Vector2(xScale, 1.0f),
				 transform);

			// top right
			rectangle = Rectangle(textureRegion.bounds.x + topLeftSlice.X() + xMid,
								  textureRegion.bounds.y,
								  bottomRightSlice.X(),
								  topLeftSlice.Y());
			
			Draw(texture,
				 Vector2(topLeftSlice.X() + xMidScaled, 0),
				 &rectangle,
				 Vector2::ZERO,
				 0,
				 color,
				 Vector2(1.0f, 1.0f),
				 transform);
			
			// center left
			rectangle = Rectangle(textureRegion.bounds.x,
								  textureRegion.bounds.y + topLeftSlice.Y(),
								  topLeftSlice.X(),
								  yMid);
			
			Draw(texture,
				 Vector2(0, topLeftSlice.Y()),
				 &rectangle,
				 Vector2::ZERO,
				 0,
				 color,
				 Vector2(1.0f, yScale),
				 transform);
			
			// center mid
			rectangle = Rectangle(textureRegion.bounds.x + topLeftSlice.X(),
								  textureRegion.bounds.y + topLeftSlice.Y(),
								  xMid,
								  yMid);
			
			Draw(texture,
				 Vector2(topLeftSlice.X(), topLeftSlice.Y()),
				 &rectangle,
				 Vector2::ZERO,
				 0,
				 color,
				 Vector2(xScale, yScale),
				 transform);
			
			// center right
			rectangle = Rectangle(textureRegion.bounds.x + topLeftSlice.X() + xMid,
								  textureRegion.bounds.y + topLeftSlice.Y(),
								  bottomRightSlice.X(),
								  yMid);
			
			Draw(texture,
				 Vector2(topLeftSlice.X() + xMidScaled, topLeftSlice.Y()),
				 &rectangle,
				 Vector2::ZERO,
				 0,
				 color,
				 Vector2(1.0f, yScale),
				 transform);
			
			// bottom left
			rectangle = Rectangle(textureRegion.bounds.x,
								  textureRegion.bounds.y + topLeftSlice.Y() + yMid,
								  topLeftSlice.X(),
								  bottomRightSlice.Y());
			
			Draw(texture,
				 Vector2(0, topLeftSlice.Y() + yMidScaled),
				 &rectangle,
				 Vector2::ZERO,
				 0,
				 color,
				 Vector2(1.0f, 1.0f),
				 transform);
			
			// bottom mid
			rectangle = Rectangle(textureRegion.bounds.x + topLeftSlice.X(),
								  textureRegion.bounds.y + topLeftSlice.Y() + yMid,
								  xMid,
								  bottomRightSlice.Y());
			
			Draw(texture,
				 Vector2(topLeftSlice.X(), topLeftSlice.Y() + yMidScaled),
				 &rectangle,
				 Vector2::ZERO,
				 0,
				 color,
				 Vector2(xScale, 1.0f),
				 transform);
			
			// bottom right
			rectangle = Rectangle(textureRegion.bounds.x + topLeftSlice.X() + xMid,
								  textureRegion.bounds.y + topLeftSlice.Y() + yMid,
								  bottomRightSlice.X(),
								  bottomRightSlice.Y());
			
			Draw(texture,
				 Vector2(topLeftSlice.X() + xMidScaled, topLeftSlice.Y() + yMidScaled),
				 &rectangle,
				 Vector2::ZERO,
				 0,
				 color,
				 Vector2(1.0f, 1.0f),
				 transform);			
		}
	}

	void SpriteBatch::DrawString(Font *font, Texture *texture, const std::string &s, const Vector2 &position, const Color &color)
	{
		if(pimpl->activeVertices == pimpl->maxVertices || (pimpl->currentTextureId != 0 && pimpl->currentTextureId != texture->GetTextureId()))
			pimpl->DrawBuffer();
		
		pimpl->currentTextureId = texture->GetTextureId();
		
		float x = 0, y = 0;

        Vertex *vertices = pimpl->vertices;
        int &activeVertices = pimpl->activeVertices;
		
		for(size_t i=0; i<s.size(); i++)
		{
			if(s[i] == '\n')
			{
				y += font->lineHeight;
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
			
			if(activeVertices == pimpl->maxVertices)
				pimpl->DrawBuffer();
			
			const Vector2 &origin = Vector2::ZERO;
			
			int width = cd.width;
			int height = cd.height;
			
			int textureWidth = texture->GetWidth();
			int textureHeight = texture->GetHeight();
			
			Vector3 v;
			Matrix3 transform = pimpl->batchTransform * CreateTranslationMatrix(position.X() + x + cd.offsetX, position.Y() + y + cd.offsetY);
			
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

        Vertex *vertices = pimpl->vertices;
        int &activeVertices = pimpl->activeVertices;
		
		if(activeVertices == pimpl->maxVertices || (pimpl->currentTextureId != 0 && pimpl->currentTextureId != texture->GetTextureId()))
			pimpl->DrawBuffer();
		
		pimpl->currentTextureId = texture->GetTextureId();
		
		float x = 0, y = 0;
		
		for(size_t i=0; i<s.size(); i++)
		{
			if(s[i] == '\n')
			{
				y += font->lineHeight;
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
			
			if(activeVertices == pimpl->maxVertices)
				pimpl->DrawBuffer();
			
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
    
    const Matrix3& SpriteBatch::GetBatchTransform() const
    {
        return pimpl->batchTransform;
    }
    
    void SpriteBatch::SetBatchTransform(const Matrix3 &transform)
    {
        pimpl->batchTransform = transform;
    }
    
    void SpriteBatch::ApplyTransform(const Matrix3 transform)
    {
        pimpl->batchTransform = transform * pimpl->batchTransform;
    }
    
    void SpriteBatch::impl::DrawBuffer()
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
		
        currentShader->SetParameter("ProjectionMatrix", currentProjectionMatrix);

        currentShader->SetParameter("TextureSampler", 0);
        
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, currentTextureId);

		glVertexAttribPointer((GLuint)ShaderAttributes::Position, 2, GL_FLOAT, 0, sizeof(Vertex), &vertices[0].x);
		glEnableVertexAttribArray((GLuint)ShaderAttributes::Position);
		glVertexAttribPointer((GLuint)ShaderAttributes::TexCoord, 2, GL_FLOAT, 0, sizeof(Vertex), &vertices[0].u);
		glEnableVertexAttribArray((GLuint)ShaderAttributes::TexCoord);
		glVertexAttribPointer((GLuint)ShaderAttributes::Color, 4, GL_FLOAT, 0, sizeof(Vertex), &vertices[0].r);
		glEnableVertexAttribArray((GLuint)ShaderAttributes::Color);
		
		glDrawArrays(GL_TRIANGLES, 0, activeVertices);
		
		activeVertices = 0;
	}
}
