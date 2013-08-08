#ifndef NINJAPARTY_DEMINAANIMATION_HPP
#define NINJAPARTY_DEMINAANIMATION_HPP

#include <string>
#include <vector>
#include <map>

#include <NinjaParty/AnimationPlayer.hpp>

#include <NinjaParty/Vector2.hpp>
#include <NinjaParty/Matrix3.hpp>

#include <NinjaParty/TextureRegion.hpp>

namespace NinjaParty
{
	class Texture;
	class SpriteBatch;

	struct Bone
	{
		std::string Name;
		
		Vector2 Position;
		float Rotation;
		
		bool VerticalFlip;
		bool HorizontalFlip;
		
		int TextureIndex;
		int ParentIndex;
		
		bool Visible;
	};
	
	struct BoneTransform
	{
		Vector2 Translation;
		float Rotation;
		Matrix3 Transform;
	};
	
	struct BoneTransitionState
	{
		Vector2 Translation;
		float Rotation;
	};
	
	struct Keyframe
	{
		std::vector<Bone> Bones;
		std::vector<Bone> UpdateOrderBones;
		
		bool VerticalFlip;
		bool HorizontalFlip;
		
		int FrameNumber;
		float FrameTime;
		
		std::string FrameTrigger;
	};
	
	struct DeminaAnimation
	{
		std::vector<Keyframe> keyframes;
		std::vector<TextureRegion> textureRegions;
		
		Texture *texture;
		
		int frameRate;
		int loopFrame;
		float loopTime;
	};
	
	class DeminaAnimationPlayer : public AnimationPlayer
	{
	public:
		DeminaAnimationPlayer();
		~DeminaAnimationPlayer();

		virtual void Start(const std::string &name, bool allowRestart = false);
		virtual void Transition(const std::string &name, float transitionTime);
		
		virtual AnimationEvent Update(float deltaSeconds);
		virtual AnimationEvent SetTime(float seconds);
		
		virtual void Draw(SpriteBatch *spriteBatch,
						  const Vector2 &position,
						  float rotation = 0,
						  const Vector2 &scale = Vector2::ONE,
						  const Color &tintColor = Color::White,
						  bool flipHorizontal = false,
						  bool flipVertical = false);
		
		void AddAnimation(const std::string &name, DeminaAnimation *deminaAnimation);
		DeminaAnimation* GetAnimation(const std::string &name);
		
	protected:
		void InsertUpdateOrderIndex(int boneIndex);
		void UpdateBoneTransforms();
		
	protected:
		std::map<std::string, DeminaAnimation*> animations;
		
		int boneCount;
		
		DeminaAnimation *currentAnimation;
		float currentAnimationTime;
		int currentKeyframeIndex;
		
		std::vector<BoneTransform> boneTransforms;
		std::vector<int> boneUpdateOrder;
		
		DeminaAnimation *transitionAnimation;
		float transitionTime;
		float transitionTotalTime;
		std::vector<BoneTransitionState> transitionStates;
	};
}

#endif//NINJAPARTY_DEMINAANIMATION_HPP
