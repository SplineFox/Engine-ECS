#pragma once

#include "Event/EventManager.h"

namespace FlatFormer { namespace Service {

	class EventManagerUser
	{
	private:
		static Event::EventManager* s_EventManagerService;

	protected:
		static Event::EventManager& GetEventManagerService()
		{
			assert(s_EventManagerService != nullptr && "ASSERT: s_EventManagerService was nullptr!");
			return *s_EventManagerService;
		}

	public:
		static void ProvideEventManagerService(Event::EventManager* eventManagerInstance)
		{
			s_EventManagerService = eventManagerInstance;
		}
	};


	using FFE_SERV_USER_EventManager = FlatFormer::Service::EventManagerUser;
}}