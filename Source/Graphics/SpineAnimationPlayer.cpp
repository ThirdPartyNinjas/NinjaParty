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
    RegionAttachment *RegionAttachment_create(const char *name, AtlasRegion *region)
    {
        throw std::runtime_error("Exception caught:\n\tFunction not implemented: RegionAttachment_create.");
    }

    AtlasPage* AtlasPage_create(const char *name, const char *path)
    {
        throw std::runtime_error("Exception caught:\n\tFunction not implemented: AtlasPage_create.");
    }
    
    char* _Util_readFile(const char *path, int *length)
    {
        return _readFile(path, length);
    }
    
    struct NinjaSkeleton
    {
        Skeleton super;
    };
    
    void _NinjaSkeleton_dispose(Skeleton *skeleton)
    {
        NinjaSkeleton *self = SUB_CAST(NinjaSkeleton, skeleton);
        _Skeleton_deinit(SUPER(self));
        FREE(self);
    }

    Skeleton* Skeleton_create(SkeletonData *data)
    {
        NinjaSkeleton *self = NEW(NinjaSkeleton);
        _Skeleton_init(SUPER(self), data);
        SUPER(self)->flipY = 1;
        VTABLE(Skeleton, self) ->dispose = _NinjaSkeleton_dispose;
        return SUPER(self);
    }

    struct TextureRegionAttachment
    {
        RegionAttachment super;
        NinjaParty::TextureRegion textureRegion;
    };
    
    void _TextureRegionAttachment_dispose(Attachment *attachment)
    {
        TextureRegionAttachment *self = SUB_CAST(TextureRegionAttachment, SUB_CAST(RegionAttachment, attachment));
        _RegionAttachment_deinit(SUPER(self));
        FREE(self);
    }
    
    void _TextureRegionAttachment_draw(Attachment *attachment, Slot *slot)
    {        
        RegionAttachment *regionAttachment = (RegionAttachment*)attachment;
        
        TextureRegionAttachment *textureRegionAttachment = (TextureRegionAttachment*)attachment;
    }
    
    void TextureRegionAttachment_drawSpriteBatch(Attachment *attachment, Slot *slot, NinjaParty::SpriteBatch *spriteBatch, const NinjaParty::Color &tintColor, NinjaParty::Texture *texture)
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
        
        NinjaParty::TexturePair texturePair(texture, textureRegionAttachment->textureRegion);
        
        spriteBatch->Draw(texturePair,
                          NinjaParty::Vector2(offsetX * m00 + offsetY * m01 + x, offsetX * m10 + offsetY * m11 + y),
                          -(slot->bone->worldRotation + regionAttachment->rotation) * 3.14159f / 180.0f,
                          NinjaParty::Color(slot->r * tintColor.R(), slot->g * tintColor.G(), slot->b * tintColor.B(), slot->a * tintColor.A()));
    }
    
    Attachment* Attachment_create(const char *name, NinjaParty::TextureRegion textureRegion)
    {
        TextureRegionAttachment *self = NEW(TextureRegionAttachment);
        self->textureRegion = textureRegion;
        _RegionAttachment_init(SUPER(self), name);
        VTABLE(Attachment, self)->dispose = _TextureRegionAttachment_dispose;
        VTABLE(Attachment, self)->draw = _TextureRegionAttachment_draw;
        return SUPER(SUPER(self));
    }
    
    struct TextureRegionAttachmentLoader
    {
        AttachmentLoader super;
        NinjaParty::TextureDictionary *textureDictionary;
    };
    
    Attachment* _TextureRegionAttachmentLoader_newAttachment(AttachmentLoader *attachmentLoader, AttachmentType type, const char *name)
    {
        TextureRegionAttachmentLoader *self = SUB_CAST(TextureRegionAttachmentLoader, attachmentLoader);
        switch(type)
        {
            case ATTACHMENT_REGION:
            {
                if(!self->textureDictionary->ContainsTexture(std::string(name) + ".png"))
                {
                    _AttachmentLoader_setError(attachmentLoader, "Region not found: ", name);
                    return nullptr;
                }

                return Attachment_create(name, self->textureDictionary->GetRegion(std::string(name) + ".png"));
            }
            default:
            {
                char buffer[16];
                sprintf(buffer, "%d", type);
                _AttachmentLoader_setError(attachmentLoader, "Unknown attachment type: ", buffer);
                return nullptr;
            }
        }
    }
    
	void _TextureRegionAttachmentLoader_dispose(AttachmentLoader *attachmentLoader)
    {
        TextureRegionAttachmentLoader* self = SUB_CAST(TextureRegionAttachmentLoader, attachmentLoader);
        _AttachmentLoader_deinit(SUPER(self));
        FREE(self);
    }
    
    AttachmentLoader* AttachmentLoader_create(NinjaParty::TextureDictionary *textureDictionary)
    {
        TextureRegionAttachmentLoader *self = NEW(TextureRegionAttachmentLoader);
        self->textureDictionary = textureDictionary;
        _AttachmentLoader_init(SUPER(self));
        VTABLE(AttachmentLoader, self)->dispose = _TextureRegionAttachmentLoader_dispose;
        VTABLE(AttachmentLoader, self)->newAttachment = _TextureRegionAttachmentLoader_newAttachment;
        return SUPER(self);
    }
    
    SkeletonData* SkeletonData_load(const char *filePath, NinjaParty::TextureDictionary *textureDictionary)
    {
        AttachmentLoader *attachmentLoader = AttachmentLoader_create(textureDictionary);
        SkeletonJson *skeletonJson = SkeletonJson_createWithLoader(attachmentLoader);
        
        int length;
        char *data = spine::_Util_readFile(filePath, &length);
        
        SkeletonData *skeletonData = SkeletonJson_readSkeletonData(skeletonJson, data);
        
        FREE(data);
        
        SkeletonJson_dispose(skeletonJson);
        AttachmentLoader_dispose(attachmentLoader);
        
        return skeletonData;
    }
}

namespace NinjaParty
{
//	class RegionAttachment: public spine::BaseRegionAttachment
//	{
//	public:
//		NinjaParty::Texture *texture;
//		NinjaParty::TextureRegion *region;
//		
//		RegionAttachment(NinjaParty::TextureRegion *region)
//            : region(region)
//        {
//        }
//		
//		virtual void updateWorldVertices(spine::Bone *bone) { }
//		virtual void draw(spine::Slot *slot) { throw; }
//        
//		virtual void draw(NinjaParty::SpriteBatch *spriteBatch, spine::Slot *slot, const NinjaParty::Color &tintColor)
//        {
//            updateOffset();
//            updateWorldVertices(slot->bone);
//            
//            float offsetX = (offset[0] + offset[4]) / 2;
//            float offsetY = (offset[1] + offset[5]) / 2;
//            
//            float m00 = slot->bone->m00;
//            float m01 = slot->bone->m01;
//            float m11 = slot->bone->m11;
//            float m10 = slot->bone->m10;
//            
//            float x = slot->bone->worldX;
//            float y = slot->bone->worldY;
//
//            NinjaParty::TexturePair texturePair(((SpineSkeleton*)(slot->skeleton))->texture, *region);
//            
//            spriteBatch->Draw(texturePair,
//                              NinjaParty::Vector2(offsetX * m00 + offsetY * m01 + x, offsetX * m10 + offsetY * m11 + y),
//                              -(slot->bone->worldRotation + rotation) * 3.14159f / 180.0f,
//                              NinjaParty::Color(slot->r * tintColor.R(), slot->g * tintColor.G(), slot->b * tintColor.B(), slot->a * tintColor.A()));
//        }
//	};
    
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
        
        spriteBatch->SetBatchTransform(transform);
  
        // todo!
//        spineSkeleton->Draw(spriteBatch, position, tintColor);
        for(int i=0; i<spineSkeleton->slotCount; i++)
        {
            if (spineSkeleton->slots[i]->attachment)
            {
                //spine::Attachment_draw(spineSkeleton->slots[i]->attachment, spineSkeleton->slots[i]);
                spine::TextureRegionAttachment_drawSpriteBatch(spineSkeleton->slots[i]->attachment, spineSkeleton->slots[i], spriteBatch, tintColor, texture);
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
