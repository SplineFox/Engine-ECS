#pragma once

#include "Engine/API.h"

namespace FlatFormer { namespace Util {

	class Timer
	{
	#pragma region Using
		using TimeClock = std::chrono::steady_clock;
		using TimePoint = TimeClock::time_point;
		using TimeDuration = std::chrono::duration<float32>;
	#pragma endregion

	private:
		TimePoint m_StartTime;
		TimeDuration m_DeltaTime;

	public:
		Timer();

		~Timer();

		void Reset();

		void Tick();

		float32 GetDeltaTime();
	};


	// example:
	//
	//Timer timer;
	//int FPS = 60;
	//while (isRunning)
	//{
	//	timer.Tick();
	//	
	//	if (timer.GetDeltaTime() >= 1 / FPS)
	//	{
	//		timer.Reset();
	//		
	//		std::cout<< 1 / timer.GetDeltaTime() <<std::endl; //FPS
	//	
	//		Update(0.1);
	//		Render();
	//	}
	//}

}}