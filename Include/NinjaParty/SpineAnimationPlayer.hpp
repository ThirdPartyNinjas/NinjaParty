#ifndef NINJAPARTY_SPINEANIMATIONPLAYER_HPP
#define NINJAPARTY_SPINEANIMATIONPLAYER_HPP

#include <map>

#include "AnimationPlayer.hpp"

namespace spine
{
	struct Animation;
	struct Skeleton;
	struct SkeletonData;
}

namespace NinjaParty
{
	class Texture;
	class TextureDictionary;
	class TextureRegion;
    
	typedef spine::Animation SpineAnimation;
	typedef spine::Skeleton SpineSkeleton;
	typedef spine::SkeletonData SpineSkeletonData;
    
	class SpineAnimationPlayer : public AnimationPlayer
	{
	public:
		SpineAnimationPlayer();
		virtual ~SpineAnimationPlayer();
		
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
		std::map<std::string, std::pair<SpineAnimation*, bool>> animations;

        SpineSkeletonData *skeletonData;
        
        SpineSkeleton *spineSkeleton;
		SpineAnimation *currentAnimation;
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
