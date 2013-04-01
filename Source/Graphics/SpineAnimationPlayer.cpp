#include <spine/BaseAttachmentLoader.h>
#include <spine/BaseSkeleton.h>
#include <spine/BaseSkeletonJson.h>
#include <spine/BaseRegionAttachment.h>
#include <spine/Bone.h>
#include <spine/Slot.h>
#include <spine/SkeletonData.h>

#include <NinjaParty/SpineAnimationPlayer.hpp>
#include <NinjaParty/SpriteBatch.hpp>
#include <NinjaParty/Texture.hpp>
#include <NinjaParty/TextureDictionary.hpp>

namespace NinjaParty
{
	class RegionAttachment: public spine::BaseRegionAttachment
	{
	public:
		NinjaParty::Texture *texture;
		NinjaParty::TextureRegion *region;
		
		RegionAttachment(NinjaParty::TextureRegion *region)
            : region(region)
        {
        }
		
		virtual void updateWorldVertices(spine::Bone *bone) { }
		virtual void draw(spine::Slot *slot) { throw; }
        
		virtual void draw(NinjaParty::SpriteBatch *spriteBatch, spine::Slot *slot, const NinjaParty::Color &tintColor)
        {
            updateOffset();
            updateWorldVertices(slot->bone);
            
            float offsetX = (offset[0] + offset[4]) / 2;
            float offsetY = (offset[1] + offset[5]) / 2;
            
            float m00 = slot->bone->m00;
            float m01 = slot->bone->m01;
            float m11 = slot->bone->m11;
            float m10 = slot->bone->m10;
            
            float x = slot->bone->worldX;
            float y = slot->bone->worldY;

            NinjaParty::TexturePair texturePair(((SpineSkeleton*)(slot->skeleton))->texture, *region);
            
            spriteBatch->Draw(texturePair,
                              NinjaParty::Vector2(offsetX * m00 + offsetY * m01 + x, offsetX * m10 + offsetY * m11 + y),
                              -(slot->bone->worldRotation + rotation) * 3.14159f / 180.0f,
                              NinjaParty::Color(slot->r * tintColor.R(), slot->g * tintColor.G(), slot->b * tintColor.B(), slot->a * tintColor.A()));
        }
	};
    
	class TextureDictionaryAttachmentLoader : public spine::BaseAttachmentLoader
	{
	public:
		NinjaParty::TextureDictionary *dictionary;
		
		TextureDictionaryAttachmentLoader(NinjaParty::TextureDictionary *dictionary)
            : dictionary(dictionary)
        {
        }
		
		virtual spine::Attachment* newAttachment(spine::AttachmentType type, const std::string &name)
        {
            switch(type)
            {
                case spine::region:
                    return new RegionAttachment(&dictionary->GetRegion(name + ".png"));
                    break;
                    
                default:
                    break;
            }
            
            return nullptr;
        }
	};
	
	SpineSkeletonLoader::SpineSkeletonLoader(NinjaParty::TextureDictionary *textureDictionary)
		: spine::BaseSkeletonJson(new TextureDictionaryAttachmentLoader(textureDictionary))
	{
		yDown = true;
	}
		
	SpineSkeletonLoader::SpineSkeletonLoader(spine::BaseAttachmentLoader *attachmentLoader)
		: spine::BaseSkeletonJson(attachmentLoader)
	{
		yDown = true;
	}
	
	SpineSkeleton::SpineSkeleton(spine::SkeletonData *skeletonData)
		: spine::BaseSkeleton(skeletonData)
	{
	}
    
    SpineSkeleton::SpineSkeleton(SpineSkeletonData *spineSkeletonData)
        : spine::BaseSkeleton(spineSkeletonData->skeletonData)
    {
    }
	
	void SpineSkeleton::Draw(NinjaParty::SpriteBatch *spriteBatch, const NinjaParty::Vector2 &position, const NinjaParty::Color &tintColor)
	{
		for(int i = 0, n = static_cast<int>(slots.size()); i < n; i++)
			if(slots[i]->attachment)
				((RegionAttachment*)slots[i]->attachment)->draw(spriteBatch, slots[i], tintColor);
	}
    
    SpineSkeletonData::SpineSkeletonData()
        : skeletonData(nullptr), skeletonLoader(nullptr)
    {
    }
    
    SpineSkeletonData::~SpineSkeletonData()
    {
        delete skeletonData;
        delete skeletonLoader;
    }
    
	
	SpineAnimationPlayer::SpineAnimationPlayer()
		: skeletonData(nullptr), currentAnimationTime(0), currentAnimation(nullptr), transitionAnimation(nullptr)
	{
	}
	
	SpineAnimationPlayer::~SpineAnimationPlayer()
	{
	}
	
	void SpineAnimationPlayer::Start(const std::string &name, bool allowRestart)
	{
        assert(skeletonData != nullptr);
        assert(spineSkeleton.get() != nullptr);
        assert(spineSkeleton->texture != nullptr);
        
		if (currentAnimationName != name || allowRestart)
		{
			currentAnimationName = name;
			currentAnimation = animations[name].first;
            currentLoop = animations[name].second;
			currentAnimationTime = 0;
			
            spineSkeleton->setToBindPose();
            spineSkeleton->updateWorldTransform();
		}
	}
    
    void SpineAnimationPlayer::Transition(const std::string &name, float transitionDelay)
    {
        assert(skeletonData != nullptr);
        assert(spineSkeleton.get() != nullptr);
        assert(spineSkeleton->texture != nullptr);
        
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
                
                currentAnimation->apply(spineSkeleton.get(), currentAnimationTime, currentLoop);
			}
            else
            {
                currentAnimation->apply(spineSkeleton.get(), currentAnimationTime, currentLoop);
                transitionAnimation->mix(spineSkeleton.get(), 0, transitionLoop, transitionTime / transitionTotalTime);
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
            
            currentAnimation->apply(spineSkeleton.get(), currentAnimationTime, currentLoop);
        }
        
        spineSkeleton->updateWorldTransform();
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
        
        spineSkeleton->Draw(spriteBatch, position, tintColor);

        spriteBatch->SetBatchTransform(currentTransform);
	}
	
	void SpineAnimationPlayer::AddAnimation(const std::string &name, SpineAnimation *spineAnimation, bool loop)
	{
        animations[name] = std::pair<SpineAnimation*, bool>(spineAnimation, loop);
	}

    void SpineAnimationPlayer::SetSkeletonData(NinjaParty::SpineSkeletonData *skeletonData)
    {
        this->skeletonData = skeletonData;
        spineSkeleton = std::unique_ptr<SpineSkeleton>(new SpineSkeleton(skeletonData));
    }
    
    void SpineAnimationPlayer::SetTexture(Texture *texture)
    {
        assert(spineSkeleton.get() != nullptr);
        
        spineSkeleton->texture = texture;
    }

    void SpineAnimationPlayer::SetSkin(const std::string &skinName)
    {
        assert(spineSkeleton.get() != nullptr);
        
        spineSkeleton->setSkin(skinName);
        spineSkeleton->setToBindPose();
    }
}
