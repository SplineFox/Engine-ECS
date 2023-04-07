#include "Memory/MemoryManager.h"

namespace FlatFormer { namespace Memory {

	MemoryManager::MemoryManager()
	{
		FFE_DEFINE_LOGGER("MemoryManager")

		m_PendingMemory.clear();

		m_MemoryUsed = 0;

		FFE_LOG_INFO("Initialized")
	}


	MemoryManager::~MemoryManager()
	{
		for (auto item : m_PendingMemory)
		{
			free((void*)item.first);
		}

		m_PendingMemory.clear();

		FFE_LOG_INFO("Released")
	}

}}