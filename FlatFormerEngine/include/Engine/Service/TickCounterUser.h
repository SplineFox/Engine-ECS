#pragma once

#include "Util/TickCounter.h"

namespace FlatFormer { namespace Service {

	class TickCounterUser
	{
	private:
		static Util::TickCounter* s_TickCounterService;

	protected:
		static Util::TickCounter& GetTickCounterService()
		{
			assert(s_TickCounterService != nullptr && "ASSERT: s_TickCounterService was nullptr!");
			return *s_TickCounterService;
		}

	public:
		static void ProvideTickCounterService(Util::TickCounter* tickCounterInstance)
		{
			s_TickCounterService = tickCounterInstance;
		}
	};


	using FFE_SERV_USER_TickCounter = FlatFormer::Service::TickCounterUser;
}}