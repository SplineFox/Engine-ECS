#pragma once

#include "Engine/API.h"

namespace FlatFormer { namespace Util {

	// The time value std::chrono::duration can be interpreted both
	// as uint and float, so the union construct is used
	union TimeStamp
	{
		float32	asFloat;
		uint32	asUInt;

		TimeStamp() : asUInt(0U)
		{}

		TimeStamp(float floatValue) : asFloat(floatValue)
		{}

		operator uint32() const { return this->asUInt; }

		inline const bool operator==(const TimeStamp& other) const { return this->asUInt == other.asUInt; }
		inline const bool operator!=(const TimeStamp& other) const { return this->asUInt != other.asUInt; }

		inline const bool operator<(const TimeStamp& other) const { return this->asFloat < other.asFloat; }
		inline const bool operator>(const TimeStamp& other) const { return this->asFloat > other.asFloat; }

	};



	class TickCounter
	{
	#pragma region MyRegion
		// duration - a type of time interval representing a number of periods,
		// which can be characterized by uint or float types.

		// The length of the duration period is measured in secondsand
		// represented as a natural fraction using std::ratio.

		// msTimeDuration - time interval in milliseconds.
		using msTimeDuration = std::chrono::duration<float32, std::milli>;
	#pragma endregion


	private:
		// The value of the time elapsed since
		// initializing TickCounter or calling Reset.
		msTimeDuration m_Elapsed;

	public:
		TickCounter();
		~TickCounter();

		void Tick(float ms);

		void Reset();

		inline TimeStamp AcquireTimeStamp() const
		{
			return TimeStamp(this->m_Elapsed.count());
		}
	};

}}