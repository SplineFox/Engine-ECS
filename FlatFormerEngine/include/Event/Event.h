#pragma once

#include "Event/IEvent.h"

namespace FlatFormer { namespace Event {

	template<class EventType>
	class Event : public IEvent
	{
	public:
		static const EventTypeID STATIC_EVENT_TYPE_ID;


		Event() : IEvent(STATIC_EVENT_TYPE_ID)
		{}
	};

	template<class EventType>
	const EventTypeID Event<EventType>::STATIC_EVENT_TYPE_ID { typeid(EventType).hash_code() };

}}