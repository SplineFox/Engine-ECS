#pragma once

#include "Event/EventDelegate.h"

namespace FlatFormer { namespace Event {

#pragma region Forward declaration
	class IEvent;
#pragma endregion

	class IEventDispatcher
	{
	public:
		virtual ~IEventDispatcher()
		{}

		virtual void Dispatch(IEvent* event) = 0;


		virtual void AddEventCallback(IEventDelegate* const eventDelegate) = 0;


		virtual void RemoveEventCallback(IEventDelegate* const eventDelegate) = 0;


		virtual inline size_t GetEventCallbackCount() const = 0;

	};

}}