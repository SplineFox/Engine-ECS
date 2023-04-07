#pragma once

#include "Util/IDPool.h"

namespace FlatFormer { namespace Service {

	class IDPoolUser
	{
	private:
		static Util::IDPool* s_IDPoolService;

	protected:
		static Util::IDPool& GetIDPoolService()
		{
			assert(s_IDPoolService != nullptr && "ASSERT: s_TickCounterService was nullptr!");
			return *s_IDPoolService;
		}

	public:
		static void ProvideIDPoolService(Util::IDPool* IDPoolInstance)
		{
			s_IDPoolService = IDPoolInstance;
		}
	};


	using FFE_SERV_USER_IDPool = FlatFormer::Service::IDPoolUser;
}}