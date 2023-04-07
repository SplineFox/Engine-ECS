#include "Util/TickCounter.h"

namespace FlatFormer { namespace Util {

	TickCounter::TickCounter() :
		m_Elapsed(msTimeDuration::zero())
	{}

	TickCounter::~TickCounter()
	{}

	void TickCounter::Tick(float ms)
	{
		this->m_Elapsed += msTimeDuration(ms);
	}

	void TickCounter::Reset()
	{
		this->m_Elapsed = msTimeDuration::zero();
	}

}}