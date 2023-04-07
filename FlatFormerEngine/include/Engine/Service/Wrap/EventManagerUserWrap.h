#pragma once

#include "Engine/Service/EventManagerUser.h"

namespace FlatFormer { namespace Service {

	class EventManagerUserWrap : public FFE_SERV_USER_EventManager
	{
	protected:

		template<class EventType>
		inline void SubscribeEvent(Event::IEventDelegate* const eventDelegate)
		{
			this->GetEventManagerService().AddEventCallback<EventType>(eventDelegate);
		}


		inline void UnsubscribeEvent(Event::IEventDelegate* eventDelegate)
		{
			this->GetEventManagerService().RemoveEventCallback(eventDelegate);
		}


		template<class EventType, class... ARGS>
		void SendEventByTrigger(ARGS&&... eventArgs)
		{
			this->GetEventManagerService().SendEventByTrigger<EventType>(std::forward<ARGS>(eventArgs)...);
		}


		template<class EventType, class... ARGS>
		void SendEventByQueue(ARGS&&... eventArgs)
		{
			this->GetEventManagerService().SendEventByQueue<EventType>(std::forward<ARGS>(eventArgs)...);
		}
	};


	using FFE_SERV_USER_WRAP_EventManager = FlatFormer::Service::EventManagerUserWrap;
}}