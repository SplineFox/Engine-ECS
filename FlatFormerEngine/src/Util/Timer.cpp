#include "Util/Timer.h"

namespace FlatFormer { namespace Util {

	Timer::Timer() :
		m_DeltaTime(TimeDuration::zero())
	{
		Reset();
	}

	Timer::~Timer()
	{}

	void Timer::Reset()
	{
		m_StartTime = TimeClock::now();
	}

	void Timer::Tick()
	{
		m_DeltaTime = TimeClock::now() - m_StartTime;
	}

	float32 Timer::GetDeltaTime()
	{
		return m_DeltaTime.count();
	}

}}