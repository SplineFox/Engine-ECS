#pragma once

constexpr auto GLOBAL_MEMORY_CAPACITY		= 58720256;		//56 mb;

#include "Engine/API.h"
#include "Memory/Allocator/PoolAllocator.h"


static constexpr size_t GENERIC_ALIGNMENT = alignof(uint8_t);

namespace FlatFormer { namespace Memory {

	class MemoryManager
	{
	#pragma region Using
		using PendingMemory = std::unordered_map<void*, std::pair<const char*, size_t>>;
	#pragma endregion

		static constexpr size_t MEMORY_CAPACITY = GLOBAL_MEMORY_CAPACITY;
	private:

		FFE_DECLARE_LOGGER

		unsigned int			m_MemoryUsed;
		PendingMemory			m_PendingMemory;

		MemoryManager(const MemoryManager&)			= delete;
		MemoryManager& operator=(MemoryManager&)	= delete;

	public:
		MemoryManager();
		~MemoryManager();

		inline void* Allocate(size_t memSize, const char* userName)
		{
			if (m_MemoryUsed + memSize > MEMORY_CAPACITY)
			{
				FFE_LOG_WARNING("Overusing system memory")
			}

			void* memPtr = malloc(memSize);

			m_PendingMemory[memPtr].first = userName;
			m_PendingMemory[memPtr].second = memSize;

			m_MemoryUsed += memSize;

			return memPtr;
		}

		void Deallocate(void* pMem)
		{
			auto iter = m_PendingMemory.find(pMem);
			if (iter == m_PendingMemory.end())
			{
				FFE_LOG_WARNING("Delete error! Pointer to release not found")
				return;
			}

			free(pMem);
			m_MemoryUsed -= iter->second.second;
			m_PendingMemory.erase(pMem);
		}
	};

}}

