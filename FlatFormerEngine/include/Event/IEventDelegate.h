#pragma once

#include "Engine/API.h"

namespace FlatFormer { namespace Event {

#pragma region Forward declaration
	class IEvent;
#pragma endregion

#pragma region Using
	using EventDelegateID = GenericID;
#pragma endregion

	class IEventDelegate
	{
	public:
		virtual inline void Invoke(const IEvent* const event) = 0;


		virtual inline EventDelegateID GetDelegateID() const = 0;


		virtual inline TypeID GetStaticEventTypeID() const = 0;


		virtual IEventDelegate* Clone() = 0;


		virtual bool operator==(const IEventDelegate* other) const = 0;
	};

}}