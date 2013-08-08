#include <stdexcept>

#include <spine/spine.h>
#include <spine/extension.h>

#include <NinjaParty/SpineAnimationPlayer.hpp>
#include <NinjaParty/SpriteBatch.hpp>
#include <NinjaParty/Texture.hpp>
#include <NinjaParty/TextureDictionary.hpp>
#include <NinjaParty/TextureRegion.hpp>

namespace spine
{    
    void _AtlasPage_createTexture(AtlasPage*, const char*)
    {
        throw std::runtime_error("_AtlasPage_createTexture not supported");
    }

    void _AtlasPage_disposeTexture(AtlasPage*)
    {
        throw std::runtime_error("_AtlasPage_disposeTexture not supported");
    }
    
    char* _Util_readFile(const char *path, int *length)
    {
        return _readFile(path, length);
    }
    
    struct TextureRegionAttachment
    {
        RegionAttachment super;
        NinjaParty::TextureRegion textureRegion;
    };
    
    void TextureRegionAttachment_draw(Attachment *attachment, Slot *slot, NinjaParty::SpriteBatch *spriteBatch, const NinjaParty::Color &tintColor, NinjaParty::Texture *texture)
    {
        RegionAttachment *regionAttachment = (RegionAttachment*)attachment;
        TextureRegionAttachment *textureRegionAttachment = (TextureRegionAttachment*)attachment;
        
        float offsetX = (regionAttachment->offset[0] + regionAttachment->offset[4]) / 2;
        float offsetY = (regionAttachment->offset[1] + regionAttachment->offset[5]) / 2;
        
        float m00 = slot->bone->m00;
        float m01 = slot->bone->m01;
        float m11 = slot->bone->m11;
        float m10 = slot->bone->m10;
        
        float x = slot->bone->worldX;
        float y = slot->bone->worldY;
        
        spriteBatch->Draw(texture,
                          textureRegionAttachment->textureRegion,
                          NinjaParty::Vector2(offsetX * m00 + offsetY * m01 + x, offsetX * m10 + offsetY * m11 + y),
                          NinjaParty::Vector2(0.5f, 0.5f),
                          -(slot->bone->worldRotation + regionAttachment->rotation) * 3.14159f / 180.0f,
                          NinjaParty::Color(slot->r * tintColor.R(), slot->g * tintColor.G(), slot->b * tintColor.B(), slot->a * tintColor.A()));
    }

    void TextureRegionAttachment_dispose(Attachment *attachment)
    {
        TextureRegionAttachment *textureRegionAttachment = (TextureRegionAttachment*)attachment;
        _Attachment_deinit(attachment);
        FREE(textureRegionAttachment);
    }
    
    TextureRegionAttachment* TextureRegionAttachment_create(const char *name, NinjaParty::TextureRegion textureRegion)
    {
        TextureRegionAttachment *textureRegionAttachment = NEW(TextureRegionAttachment);
        RegionAttachment *regionAttachment = (RegionAttachment*)textureRegionAttachment;
        Attachment *attachment = (Attachment*)textureRegionAttachment;
        
        textureRegionAttachment->textureRegion = textureRegion;
        
        regionAttachment->scaleX = 1.0f;
        regionAttachment->scaleY = 1.0f;

        _Attachment_init(attachment, name, ATTACHMENT_REGION, &TextureRegionAttachment_dispose);

        return textureRegionAttachment;
    }
    
    struct TextureRegionAttachmentLoader
    {
        AttachmentLoader super;
        NinjaParty::TextureDictionary *textureDictionary;
    };

    Attachment* TextureRegionAttachmentLoader_newAttachment(AttachmentLoader *attachmentLoader, Skin *skin, AttachmentType type, const char *name)
    {
        TextureRegionAttachmentLoader *textureRegionAttachmentLoader = (TextureRegionAttachmentLoader*)attachmentLoader;
        
        switch(type)
        {
            case ATTACHMENT_REGION:
                if(!textureRegionAttachmentLoader->textureDictionary->ContainsTexture(std::string(name) + ".png"))
                {
                    _AttachmentLoader_setError(attachmentLoader, "Region not found: ", name);
                    return nullptr;
                }
                else
                {
                    NinjaParty::TextureRegion textureRegion = textureRegionAttachmentLoader->textureDictionary->GetRegion(std::string(name) + ".png");
                    RegionAttachment *regionAttachment = (RegionAttachment*)TextureRegionAttachment_create(name, textureRegion);
                    
                    // note: We already take care of finding the center of the original image
                    //  So just pass the size of the original data here
                    
                    regionAttachment->regionOffsetX = 0;
                    regionAttachment->regionOffsetY = 0;
                    regionAttachment->regionWidth = textureRegion.input.width;
                    regionAttachment->regionHeight = textureRegion.input.height;
                    regionAttachment->regionOriginalWidth = textureRegion.input.width;
                    regionAttachment->regionOriginalHeight = textureRegion.input.height;
                    
                    return (Attachment*)regionAttachment;
                }
                
            default:
                _AttachmentLoader_setUnknownTypeError(attachmentLoader, type);
                return nullptr;
        }
    }

	void TextureRegionAttachmentLoader_dispose(AttachmentLoader *attachmentLoader)
    {
        TextureRegionAttachmentLoader *textureRegionAttachmentLoader = (TextureRegionAttachmentLoader*)attachmentLoader;
        _AttachmentLoader_deinit(attachmentLoader);
        FREE(textureRegionAttachmentLoader);
    }
    
    TextureRegionAttachmentLoader* TextureRegionAttachmentLoader_create(NinjaParty::TextureDictionary *textureDictionary)
    {
        TextureRegionAttachmentLoader *textureRegionAttachmentLoader = NEW(TextureRegionAttachmentLoader);
        AttachmentLoader *attachmentLoader = (AttachmentLoader*)textureRegionAttachmentLoader;
        
        textureRegionAttachmentLoader->textureDictionary = textureDictionary;
        _AttachmentLoader_init(attachmentLoader, &TextureRegionAttachmentLoader_dispose, &TextureRegionAttachmentLoader_newAttachment);
        
        return textureRegionAttachmentLoader;
    }
    
    SkeletonData* SkeletonData_loadBuffer(const unsigned char *buffer, int size, NinjaParty::TextureDictionary *textureDictionary)
    {
        TextureRegionAttachmentLoader *textureRegionAttachmentLoader = TextureRegionAttachmentLoader_create(textureDictionary);
        AttachmentLoader *attachmentLoader = (AttachmentLoader*)textureRegionAttachmentLoader;
        
        SkeletonJson *skeletonJson = SkeletonJson_createWithLoader(attachmentLoader);
        SkeletonData *skeletonData = SkeletonJson_readSkeletonData(skeletonJson, (const char*)buffer);
        
        SkeletonJson_dispose(skeletonJson);
        AttachmentLoader_dispose(attachmentLoader);
        
        return skeletonData;
    }

    SkeletonData* SkeletonData_loadFile(const char *filePath, NinjaParty::TextureDictionary *textureDictionary)
    {
        int size;
        const unsigned char *buffer = (const unsigned char*)spine::_Util_readFile(filePath, &size);

        SkeletonData *skeletonData = SkeletonData_loadBuffer(buffer, size, textureDictionary);
        FREE(buffer);

        return skeletonData;
    }
}

namespace NinjaParty
{
	SpineAnimationPlayer::SpineAnimationPlayer()
        : skeletonData(nullptr), spineSkeleton(nullptr), currentAnimationTime(0), currentAnimation(nullptr), transitionAnimation(nullptr), texture(nullptr)
	{
	}
	
	SpineAnimationPlayer::~SpineAnimationPlayer()
	{
        if(spineSkeleton != nullptr)
        {
            spine::Skeleton_dispose(spineSkeleton);
        }
	}
	
	void SpineAnimationPlayer::Start(const std::string &name, bool allowRestart)
	{
        assert(skeletonData != nullptr);
        assert(spineSkeleton != nullptr);
        assert(texture != nullptr);
        
		if (currentAnimationName != name || allowRestart)
		{
			currentAnimationName = name;
			currentAnimation = animations[name].first;
            currentLoop = animations[name].second;
			currentAnimationTime = 0;
            
            spine::Skeleton_setToBindPose(spineSkeleton);
            spine::Skeleton_updateWorldTransform(spineSkeleton);
		}
	}
    
    void SpineAnimationPlayer::Transition(const std::string &name, float transitionDelay)
    {
        assert(skeletonData != nullptr);
        assert(spineSkeleton != nullptr);
        assert(texture != nullptr);
        
        inTransition = true;
        transitionAnimationName = name;
        transitionAnimation = animations[name].first;
        transitionLoop = animations[name].second;
        transitionTime = 0;
        transitionTotalTime = transitionDelay;
    }

	AnimationEvent SpineAnimationPlayer::Update(float deltaSeconds)
	{
        AnimationEvent animationEvent = { false, "" };

        if(currentAnimation == nullptr)
            return animationEvent;
        
        deltaSeconds *= timeScale;
        
        if(inTransition)
        {
            transitionTime += deltaSeconds;
            
			if (transitionTime >= transitionTotalTime)
			{
				inTransition = false;
				
				currentAnimation = transitionAnimation;
				currentAnimationName = transitionAnimationName;
                currentLoop = transitionLoop;
				currentAnimationTime = 0;
                
                spine::Animation_apply(currentAnimation, spineSkeleton, currentAnimationTime, currentLoop);
			}
            else
            {
                spine::Animation_apply(currentAnimation, spineSkeleton, currentAnimationTime, currentLoop);
                spine::Animation_mix(transitionAnimation, spineSkeleton, 0, transitionLoop, transitionTime / transitionTotalTime);
            }
        }
        else
        {
            float timeBefore = currentAnimationTime;
            
            currentAnimationTime += deltaSeconds;
            
            if(currentLoop)
            {
                if(currentAnimationTime > currentAnimation->duration)
                {
                    currentAnimationTime = fmodf(currentAnimationTime, currentAnimation->duration);
                }
                else if(currentAnimationTime < 0)
                {
                    currentAnimationTime += currentAnimation->duration;
                }
            }
            else
            {
                if(currentAnimationTime < 0)
                {
                    currentAnimationTime = 0;
                }
                else if(timeBefore < currentAnimation->duration && currentAnimationTime >= currentAnimation->duration)
                {
                    animationEvent.AnimationComplete = true;
                }
            }
            
            spine::Animation_apply(currentAnimation, spineSkeleton, currentAnimationTime, currentLoop);
        }
        
        spine::Skeleton_updateWorldTransform(spineSkeleton);
        return animationEvent;
	}
	
	AnimationEvent SpineAnimationPlayer::SetTime(float seconds)
	{
		AnimationEvent animationEvent = { false, "" };
		
		if(currentAnimation == nullptr)
			return animationEvent;
		
		if(inTransition)
		{
			inTransition = false;
			
			currentAnimation = transitionAnimation;
			currentAnimationName = transitionAnimationName;
		}
		
		currentAnimationTime = seconds;

        // todo: need animation events once they're added to Spine
		return animationEvent;
	}

	void SpineAnimationPlayer::Draw(SpriteBatch *spriteBatch, const Vector2 &position, float rotation,
				const Vector2 &scale, const Color &tintColor, bool flipHorizontal, bool flipVertical)
	{
        if(currentAnimation == nullptr)
            return;
        
        Matrix3 currentTransform = spriteBatch->GetBatchTransform();
        
        Matrix3 transform = NinjaParty::CreateTranslationMatrix(position.X(), position.Y()) *
                    NinjaParty::CreateRotationMatrix(rotation) *
                    NinjaParty::CreateScaleMatrix(scale.X() * (flipHorizontal ? -1 : 1), scale.Y() * (flipVertical ? -1 : 1));
        
        spriteBatch->SetBatchTransform(currentTransform * transform);
  
        for(int i=0; i<spineSkeleton->slotCount; i++)
        {
            if (spineSkeleton->slots[i]->attachment)
            {
                spine::TextureRegionAttachment_draw(spineSkeleton->slots[i]->attachment, spineSkeleton->slots[i], spriteBatch, tintColor, texture);
            }
        }

        spriteBatch->SetBatchTransform(currentTransform);
	}
	
	void SpineAnimationPlayer::AddAnimation(const std::string &name, SpineAnimation *spineAnimation, bool loop)
	{
        animations[name] = std::pair<SpineAnimation*, bool>(spineAnimation, loop);
	}

    void SpineAnimationPlayer::SetSkeletonData(NinjaParty::SpineSkeletonData *skeletonData)
    {
        this->skeletonData = skeletonData;
        if(spineSkeleton != nullptr)
            spine::Skeleton_dispose(spineSkeleton);
        spineSkeleton = spine::Skeleton_create(skeletonData);
        spineSkeleton->flipY = 1;
    }
    
    void SpineAnimationPlayer::SetTexture(Texture *texture)
    {
        this->texture = texture;
    }

    void SpineAnimationPlayer::SetSkin(const std::string &skinName)
    {
        assert(spineSkeleton != nullptr);
        
        spine::Skeleton_setSkinByName(spineSkeleton, skinName.c_str());
        spine::Skeleton_setToBindPose(spineSkeleton);
    }
}
