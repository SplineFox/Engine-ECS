#pragma once

#include "Memory/MemoryManager.h"

namespace FlatFormer { namespace Service {

	class MemoryManagerUser
	{
	private:
		static Memory::MemoryManager* s_MemoryManagerService;

	protected:
		static Memory::MemoryManager& GetMemoryManagerService()
		{
			assert(s_MemoryManagerService != nullptr && "ASSERT: s_MemoryManagerService was nullptr!");
			return *s_MemoryManagerService;
		}

	public:
		static void ProvideMemoryManagerService(Memory::MemoryManager* memoryManagerInstance)
		{
			s_MemoryManagerService = memoryManagerInstance;
		}
	};


	using FFE_SERV_USER_MemoryManager = FlatFormer::Service::MemoryManagerUser;
}}