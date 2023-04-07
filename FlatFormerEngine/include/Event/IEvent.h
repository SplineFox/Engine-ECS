#pragma once

#include "Engine/API.h"
#include "Engine/Service/TickCounterUser.h"

namespace FlatFormer { namespace Event {

	#pragma region MyRegion
	using EventTypeID = TypeID;
	using EventTimestamp = Util::TimeStamp;
	#pragma endregion

	static constexpr EventTypeID INVALID_EVENT_TYPE = INVALID_TYPE_ID;


	class IEvent : public Service::FFE_SERV_USER_TickCounter
	{
	private:
		EventTypeID m_EventTypeID;
		EventTimestamp m_TimeStamp;

	public:
		IEvent(EventTypeID eventTypeID);

		inline const EventTypeID		GetEventTypeID() const { return this->m_EventTypeID; }
		
		inline const EventTimestamp		GetTimeStamp()	 const { return m_TimeStamp; }
	};

}}

