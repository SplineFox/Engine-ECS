#include "ECS/ComponentManager.h"

namespace FlatFormer { namespace ECS {

	ComponentManager::ComponentManager() :
		m_ComponentDispatcherRegistry(Util::TypeIDCounter<IComponent>::GetCount(), nullptr)
	{
		FFE_DEFINE_LOGGER("ComponentManager")
		FFE_LOG_INFO("Initialized")
	}


	ComponentManager::~ComponentManager()
	{
		for (auto dispatcher : m_ComponentDispatcherRegistry)
		{
			delete dispatcher;
			dispatcher = nullptr;
		}

		FFE_LOG_INFO("Released");
	}

}}