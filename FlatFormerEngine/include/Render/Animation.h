#pragma once

#include "Engine/API.h"
#include "Render/Sprite.h"

namespace FlatFormer {

	class Animation
	{
	public:
		struct KeyFrame
		{
		public:
			Sprite sprite;
			float frameDuration;

			KeyFrame(Sprite frame, float frameDuration) :
				sprite(frame),
				frameDuration(frameDuration)
			{}
		};

		using KeyFrameVector = std::vector<KeyFrame>;
	private:
		KeyFrameVector keyFrames;
		int currentKeyFrame;
		float stateTime;
		bool isLooped;

	public:
		Animation() :
			keyFrames(),
			currentKeyFrame(0),
			stateTime(0.0f),
			isLooped(true)
		{}

		Animation(KeyFrameVector keyFrames) :
			keyFrames(keyFrames),
			currentKeyFrame(0),
			stateTime(0.0f),
			isLooped(true)
		{}


		Animation(const Animation& animation)
		{
			keyFrames = animation.keyFrames;
			currentKeyFrame = animation.currentKeyFrame;
			stateTime = animation.stateTime;
			isLooped = animation.isLooped;
		}

		void AddFrame(Sprite sprite, float duration)
		{
			keyFrames.emplace_back(sprite, duration);
		}


		void UpdateState(float deltaTime)
		{
			stateTime += deltaTime;
			auto currnetFrameDuration = keyFrames.at(currentKeyFrame).frameDuration;
			if (stateTime >= currnetFrameDuration)
			{
				if (currentKeyFrame + 1 >= keyFrames.size())
				{
					if (isLooped)
						currentKeyFrame = 0;
				}
				else
					currentKeyFrame++;
				stateTime = 0;
			}
		}


		void ResetKeyFrame()
		{
			currentKeyFrame = 0;
			stateTime = 0;
		}


		float GetStateTime()
		{
			return stateTime;
		}


		bool IsLooped()
		{
			return isLooped;
		}


		void ToogleIsLooped()
		{
			isLooped = !isLooped;
		}


		Sprite GetCurrentFrame()
		{
			return keyFrames.at(currentKeyFrame).sprite;
		}

	};

}