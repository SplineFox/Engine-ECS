#pragma once

#include "ECS/ComponentManager.h"

namespace FlatFormer { namespace Service {

	class ComponentManagerUser
	{
	private:
		static ECS::ComponentManager* s_ComponentManagerService;

	protected:
		static ECS::ComponentManager& GetComponentManagerService()
		{
			assert(s_ComponentManagerService != nullptr && "ASSERT: s_ComponentManagerService was nullptr!");
			return *s_ComponentManagerService;
		}

	public:
		static void ProvideComponentManagerService(ECS::ComponentManager* componentManagerInstance)
		{
			s_ComponentManagerService = componentManagerInstance;
		}
	};


	using FFE_SERV_USER_ComponentManager = FlatFormer::Service::ComponentManagerUser;
}}