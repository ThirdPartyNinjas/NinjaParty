#include <algorithm>
#include <stdexcept>

#include <NinjaParty/MathHelpers.hpp>

#include <NinjaParty/DeminaAnimationPlayer.hpp>
#include <NinjaParty/SpriteBatch.hpp>
#include <NinjaParty/Texture.hpp>

namespace NinjaParty
{
	DeminaAnimationPlayer::DeminaAnimationPlayer()
		: boneCount(-1), currentAnimation(nullptr)
	{
	}

	DeminaAnimationPlayer::~DeminaAnimationPlayer()
	{
	}
	
	void DeminaAnimationPlayer::InsertUpdateOrderIndex(int boneIndex)
	{
		if(std::find(boneUpdateOrder.begin(), boneUpdateOrder.end(), boneIndex) != boneUpdateOrder.end())
			return;
		
		Bone &bone = currentAnimation->keyframes[0].Bones[boneIndex];
		if(bone.ParentIndex != -1)
			InsertUpdateOrderIndex(bone.ParentIndex);
		boneUpdateOrder.push_back(boneIndex);
	}

	void DeminaAnimationPlayer::UpdateBoneTransforms()
	{
		Keyframe *lerpKeyframe;
		float t;
		float time = currentAnimationTime - currentAnimation->keyframes[currentKeyframeIndex].FrameTime;
		
		if(inTransition)
		{
			lerpKeyframe = &transitionAnimation->keyframes[0];
			t = transitionTime / transitionTotalTime;
			
			for(int i=0; i<boneCount; i++)
			{
				int currentBone = boneUpdateOrder[i];
				
				Bone &lerpBone = lerpKeyframe->Bones[currentBone];
				
				boneTransforms[currentBone].Rotation = Lerp(transitionStates[currentBone].Rotation, lerpBone.Rotation, t);
				boneTransforms[currentBone].Translation = Lerp(Vector2(transitionStates[currentBone].Translation.X(), transitionStates[currentBone].Translation.Y()), Vector2(lerpBone.Position.X(), lerpBone.Position.Y()), t);
				Matrix3 parentMatrix = currentAnimation->keyframes[currentKeyframeIndex].Bones[currentBone].ParentIndex == -1 ? Matrix3::IDENTITY : boneTransforms[currentAnimation->keyframes[currentKeyframeIndex].Bones[currentBone].ParentIndex].Transform;
				boneTransforms[currentBone].Transform =  parentMatrix * CreateTranslationMatrix(boneTransforms[currentBone].Translation.X(), boneTransforms[currentBone].Translation.Y()) * CreateRotationMatrix(boneTransforms[currentBone].Rotation);
			}
			
			return;
		}
		
		if (currentKeyframeIndex == currentAnimation->keyframes.size() - 1)
		{
			lerpKeyframe = &currentAnimation->keyframes[0];
			
			if (currentAnimation->loopFrame != -1)
				t = time / (currentAnimation->loopTime - currentAnimation->keyframes[currentKeyframeIndex].FrameTime);
			else
				t = 0;
		}
		else
		{
			lerpKeyframe = &currentAnimation->keyframes[currentKeyframeIndex + 1];
			t = time / (lerpKeyframe->FrameTime - currentAnimation->keyframes[currentKeyframeIndex].FrameTime);
		}
		
		for(int i=0; i<boneCount; i++)
		{
			int currentBone = boneUpdateOrder[i];
			
			Bone &bone = currentAnimation->keyframes[currentKeyframeIndex].Bones[currentBone];
			Bone &lerpBone = lerpKeyframe->Bones[currentBone];
			
			boneTransforms[currentBone].Rotation = Lerp(bone.Rotation, lerpBone.Rotation, t);
			boneTransforms[currentBone].Translation = Lerp(Vector2(bone.Position.X(), bone.Position.Y()), Vector2(lerpBone.Position.X(), lerpBone.Position.Y()), t);
			Matrix3 parentMatrix = currentAnimation->keyframes[currentKeyframeIndex].Bones[currentBone].ParentIndex == -1 ? Matrix3::IDENTITY : boneTransforms[currentAnimation->keyframes[currentKeyframeIndex].Bones[currentBone].ParentIndex].Transform;
			boneTransforms[currentBone].Transform =  parentMatrix * CreateTranslationMatrix(boneTransforms[currentBone].Translation.X(), boneTransforms[currentBone].Translation.Y()) * CreateRotationMatrix(boneTransforms[currentBone].Rotation);
		}
	}

	void DeminaAnimationPlayer::AddAnimation(const std::string &name, DeminaAnimation *deminaAnimation)
	{
		if(boneCount != -1 && boneCount != deminaAnimation->keyframes[0].Bones.size())
			throw std::runtime_error(name + " invalid bone count");
		
		animations[name] = deminaAnimation;
		
		if(boneCount == -1)
		{
			boneCount = static_cast<int>(deminaAnimation->keyframes[0].Bones.size());
			currentAnimation = deminaAnimation;
			currentAnimationName = name;
			currentAnimationTime = 0;
			currentKeyframeIndex = 0;
			
			boneTransforms.clear();
			boneUpdateOrder.clear();
			
			BoneTransform bt;
			bt.Rotation = 0;
			bt.Translation = Vector2::ZERO;
			bt.Transform = Matrix3::IDENTITY;
			
			BoneTransitionState bts;
			bts.Rotation = 0;
			bts.Translation = Vector2::ZERO;
			
			for(int i=0; i<boneCount; i++)
			{
				boneTransforms.push_back(bt);
				InsertUpdateOrderIndex(i);
				transitionStates.push_back(bts);
			}
		}
	}

	AnimationEvent DeminaAnimationPlayer::Update(float deltaSeconds)
	{
		deltaSeconds *= timeScale;
		
		AnimationEvent animationEvent = { false, "" };
		
		if (currentAnimation == nullptr)
			return animationEvent;
		
		int startKeyframeIndex = currentKeyframeIndex;
		
		if (inTransition)
		{
			transitionTime += deltaSeconds;
			
			if (transitionTime > transitionTotalTime)
			{
				inTransition = false;
				
				currentAnimation = transitionAnimation;
				currentAnimationName = transitionAnimationName;
				// note: the next line is more correct, but setting to zero dodges errors from long delays
				//currentAnimationTime = transitionTime - transitionTotalTime;
				currentAnimationTime = 0;
				currentKeyframeIndex = 0;
			}
		}
		else
		{
			bool reachedEnd = false;
			
			currentAnimationTime += deltaSeconds;
			
			if (currentKeyframeIndex == currentAnimation->keyframes.size() - 1)
			{
				if (currentAnimation->loopFrame != -1)
				{
					if (currentAnimationTime > currentAnimation->loopTime)
					{
						currentAnimationTime -= currentAnimation->loopTime;
						currentKeyframeIndex = 0;
					}
				}
				else
				{
					currentAnimationTime = currentAnimation->keyframes[currentKeyframeIndex].FrameTime;
					reachedEnd = true;
				}
			}
			else
			{
				if (currentAnimationTime > currentAnimation->keyframes[currentKeyframeIndex + 1].FrameTime)
				{
					// note: this line introduces a slight timing error, but dodges errors caused by very long delays
					currentAnimationTime = currentAnimation->keyframes[currentKeyframeIndex + 1].FrameTime;
					currentKeyframeIndex++;
				}
			}
			
			animationEvent.AnimationComplete = reachedEnd;
		}
		
		UpdateBoneTransforms();
		
		// Set the frame trigger, could be setting it to "" if no event, which is fine
		if (currentKeyframeIndex != startKeyframeIndex)
			animationEvent.AnimationEventName = currentAnimation->keyframes[currentKeyframeIndex].FrameTrigger;
		
		return animationEvent;
	}

	AnimationEvent DeminaAnimationPlayer::SetTime(float seconds)
	{
		AnimationEvent animationEvent = { false, "" };
		
		if (currentAnimation == nullptr)
			return animationEvent;
		
		if (inTransition)
		{
			inTransition = false;
			
			currentAnimation = transitionAnimation;
			currentAnimationName = transitionAnimationName;
		}
		
		currentAnimationTime = seconds;
		currentKeyframeIndex = 0;
		
		int keyframeCount = static_cast<int>(currentAnimation->keyframes.size());
		for(int i=0; i<keyframeCount; i++)
		{
			if(currentAnimation->keyframes[i].FrameTime > seconds)
				break;
			currentKeyframeIndex = i;
		}
		
		UpdateBoneTransforms();
		
		animationEvent.AnimationEventName = currentAnimation->keyframes[currentKeyframeIndex].FrameTrigger;
		
		return animationEvent;
	}

	void DeminaAnimationPlayer::Draw(SpriteBatch *spriteBatch, const Vector2 &position, float rotation, const Vector2 &scale, const Color &tintColor, bool flipHorizontal, bool flipVertical)
	{
		if(currentAnimation == nullptr)
			return;
		
		Matrix3 scaleMatrix = CreateScaleMatrix(scale.X(), scale.Y());
		
		if(BoolXor(flipHorizontal, currentAnimation->keyframes[currentKeyframeIndex].HorizontalFlip))
			scaleMatrix = CreateScaleMatrix(-1, 1);
		if(BoolXor(flipVertical, currentAnimation->keyframes[currentKeyframeIndex].VerticalFlip))
			scaleMatrix = CreateScaleMatrix(1, -1) * scaleMatrix;
		
		for(int i=0; i<boneCount; i++)
		{
			bool bvf = currentAnimation->keyframes[currentKeyframeIndex].Bones[i].VerticalFlip;
			bool bhf = currentAnimation->keyframes[currentKeyframeIndex].Bones[i].HorizontalFlip;

			spriteBatch->Draw(TexturePair(currentAnimation->texture, currentAnimation->textureRegions[currentAnimation->keyframes[currentKeyframeIndex].Bones[i].TextureIndex]),
							  Vector2::ZERO, 0, tintColor, Vector2(bhf ? -1.0f : 1.0f, bvf ? -1.0f : 1.0f),
							  CreateTranslationMatrix(position.X(), position.Y()) * CreateRotationMatrix(rotation) * scaleMatrix * boneTransforms[i].Transform);
		}
	}

	void DeminaAnimationPlayer::Start(const std::string &name, bool allowRestart)
	{
		if (currentAnimationName != name || allowRestart)
		{
			inTransition = false;
			
			currentAnimationName = name;
			currentAnimation = animations[name];
			currentAnimationTime = 0;
			currentKeyframeIndex = 0;
			
			Update(0);
		}
	}
	
	void DeminaAnimationPlayer::Transition(const std::string &name, float transitionDelay)
	{
		for(int i=0; i<boneCount; i++)
		{
			transitionStates[i].Rotation = boneTransforms[i].Rotation;
			transitionStates[i].Translation = boneTransforms[i].Translation;
		}
		
		inTransition = true;
		transitionTime = 0;
		transitionTotalTime = transitionDelay;
		transitionAnimationName = name;
		transitionAnimation = animations[name];
	}
}


