#pragma once

#include "ECS/Component.h"

#include "Render/Animation.h"

namespace FlatFormer {

	class AnimatorComponent : public ECS::Component<AnimatorComponent>
	{
	public:
		bool enabled;
		FlipType flipType;
		ColorRGBA color;

		std::map<std::string, Animation> animations;
		Animation* currentAnimation;

	public:
		AnimatorComponent(std::string animationName, Animation animation) :
			enabled(true),
			flipType(FlipType::None)
		{
			animations[animationName] = animation;
			currentAnimation = &(animations.at(animationName));
		}


		~AnimatorComponent()
		{}


		void AddAnimation(std::string animationName, Animation animation)
		{
			animations[animationName] = animation;
			currentAnimation = &(animations.at(animationName));
		}


		void Update(float deltaTime)
		{
			currentAnimation->UpdateState(deltaTime);
		}


		Animation& GetCurrnetAnimation()
		{
			return *currentAnimation;
		}


		void SetCurrentAnimation(std::string animationName)
		{
			currentAnimation = &(animations.at(animationName));
		}


		void Flip(FlipType flip)
		{
			flipType = flip;
		}


		FlipType GetFlipType()
		{
			return flipType;
		}


		bool IsEnabled()
		{
			return enabled;
		}


		void ToggleEnabled()
		{
			enabled = !enabled;
		}
	};

}