#ifndef NINJAPARTY_SPINEANIMATIONPLAYER_HPP
#define NINJAPARTY_SPINEANIMATIONPLAYER_HPP

#include <map>

#include <spine/spine.h>

#include "AnimationPlayer.hpp"

namespace NinjaParty
{
	class Texture;
	class TextureDictionary;
	class TextureRegion;
    
	typedef spine::SkeletonData SpineSkeletonData;
	typedef spine::Animation SpineAnimation;
    
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
        
        spine::Skeleton *spineSkeleton;
		spine::Animation *currentAnimation;
		float currentAnimationTime;
        bool currentLoop;
        
		SpineAnimation *transitionAnimation;
		float transitionTime;
		float transitionTotalTime;
        bool transitionLoop;
        
        Texture *texture;
	};
}

#endif//NINJAPARTY_SPINEANIMATIONPLAYER_HPP
