#pragma once

#include "ECS/SystemManager.h"

namespace FlatFormer { namespace Service {

	class SystemManagerUser
	{
	private:
		static ECS::SystemManager* s_SystemManagerService;

	protected:
		static ECS::SystemManager& GetSystemManagerService()
		{
			assert(s_SystemManagerService != nullptr && "ASSERT: s_SystemManagerService was nullptr!");
			return *s_SystemManagerService;
		}

	public:
		static void ProvideSystemManagerService(ECS::SystemManager* systemManagerInstance)
		{
			s_SystemManagerService = systemManagerInstance;
		}
	};


	using FFE_SERV_USER_SystemManager = FlatFormer::Service::SystemManagerUser;
}}