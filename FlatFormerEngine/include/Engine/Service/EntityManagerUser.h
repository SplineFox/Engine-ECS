#pragma once

#include "ECS/EntityManager.h"

namespace FlatFormer { namespace Service {

	class EntityManagerUser
	{
	private:
		static ECS::EntityManager* s_EntityManagerService;

	protected:
		static ECS::EntityManager& GetEntityManagerService()
		{
			assert(s_EntityManagerService != nullptr && "ASSERT: s_EntityManagerService was nullptr!");
			return *s_EntityManagerService;
		}

	public:
		static void ProvideEntityManagerService(ECS::EntityManager* entityManagerInstance)
		{
			s_EntityManagerService = entityManagerInstance;
		}
	};


	using FFE_SERV_USER_EntityManager = FlatFormer::Service::EntityManagerUser;
}}