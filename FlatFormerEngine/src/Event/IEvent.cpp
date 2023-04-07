#include "Event/IEvent.h"

namespace FlatFormer { namespace Event {

	IEvent::IEvent(EventTypeID eventTypeID) :
		m_EventTypeID(eventTypeID)
	{
		m_TimeStamp = this->GetTickCounterService().AcquireTimeStamp();
	}

}}