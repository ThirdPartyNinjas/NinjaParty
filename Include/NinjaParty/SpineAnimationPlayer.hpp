#ifndef NINJAPARTY_SPINEANIMATIONPLAYER_HPP
#define NINJAPARTY_SPINEANIMATIONPLAYER_HPP

#include <map>

#include <spine/Animation.h>
#include <spine/BaseAttachmentLoader.h>
#include <spine/BaseRegionAttachment.h>
#include <spine/BaseSkeleton.h>
#include <spine/BaseSkeletonJson.h>

#include "AnimationPlayer.hpp"

namespace NinjaParty
{
	class Texture;
	class TextureDictionary;
	class TextureRegion;
    
    struct SpineSkeletonData;
	
	typedef spine::Animation SpineAnimation;

	class SpineSkeleton: public spine::BaseSkeleton
	{
	public:
		NinjaParty::Texture *texture;
		
		SpineSkeleton(spine::SkeletonData *skeletonData);
        SpineSkeleton(SpineSkeletonData *spineSkeletonData);
        
		void Draw(NinjaParty::SpriteBatch *spriteBatch, const NinjaParty::Vector2 &position, const NinjaParty::Color &tintColor);
	};
	
	class SpineSkeletonLoader: public spine::BaseSkeletonJson
	{
	public:
		SpineSkeletonLoader(NinjaParty::TextureDictionary *textureDictionary);
		SpineSkeletonLoader(spine::BaseAttachmentLoader *attachmentLoader);
	};
	
	struct SpineSkeletonData
	{
		SpineSkeletonData();
		~SpineSkeletonData();
		
        spine::SkeletonData *skeletonData;
		SpineSkeletonLoader *skeletonLoader;
	};
	
	class SpineAnimationPlayer : public AnimationPlayer
	{
	public:
		SpineAnimationPlayer();
		~SpineAnimationPlayer();
		
		virtual void Start(const std::string &name, bool allowRestart = false);
		virtual void Transition(const std::string &name, float transitionDelay);
		
		virtual AnimationEvent Update(float deltaSeconds);
		virtual AnimationEvent SetTime(float seconds);
		
		virtual void Draw(SpriteBatch *spriteBatch,
						  const Vector2 &position,
						  float rotation = 0,
						  const Vector2 &scale = Vector2::ONE,
						  const Color &tintColor = Color::White,
						  bool flipHorizontal = false,
						  bool flipVertical = false);
		
		void AddAnimation(const std::string &name, SpineAnimation *spineAnimation, bool loop);
		
        void SetSkeletonData(SpineSkeletonData *skeletonData);
        void SetTexture(Texture *texture);
        void SetSkin(const std::string &skinName);
        
	protected:
		std::map<std::string, std::pair<spine::Animation*, bool>> animations;

        SpineSkeletonData *skeletonData;
        std::unique_ptr<SpineSkeleton> spineSkeleton;
		spine::Animation *currentAnimation;
		float currentAnimationTime;
        bool currentLoop;
        
		SpineAnimation *transitionAnimation;
		float transitionTime;
		float transitionTotalTime;
        bool transitionLoop;
	};
}

#endif//NINJAPARTY_SPINEANIMATIONPLAYER_HPP
