#include "ECS/EntityManager.h"

namespace FlatFormer { namespace ECS {

	EntityManager::EntityManager()
	{
		FFE_DEFINE_LOGGER("EntityManager")
		FFE_LOG_INFO("Initialized")
	}


	EntityManager::~EntityManager()
	{
		FFE_LOG_INFO("Released");
	}
}}