#include "ECS\SystemManager.h"

namespace FlatFormer { namespace ECS {

	SystemManager::SystemManager()
	{
		m_SystemAllocator = new SystemAllocator(8388608, this->GetMemoryManagerService().Allocate(8388608, "SystemManager"));
		
		FFE_DEFINE_LOGGER("SystemManager")
		FFE_LOG_INFO("Initialize")
	}


	SystemManager::~SystemManager()
	{
		this->GetMemoryManagerService().Deallocate((void*)m_SystemAllocator->GetMemoryAddress0());
		m_SystemAllocator = nullptr;
	}

}}