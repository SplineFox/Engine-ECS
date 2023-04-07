#pragma once

#include "Memory/Allocator/IAllocator.h"

namespace FlatFormer {namespace Memory {namespace Allocator {

	class LinearAllocator : public IAllocator
	{
	private:
		FFE_DECLARE_LOGGER

	public:

		LinearAllocator(size_t memSize, const void* mem);

		virtual ~LinearAllocator();
		
		virtual void*	Allocate	(size_t size, uint8_t alignment)	override;
		virtual void*	Allocate	(size_t size)						override;
		virtual void	Deallocate	(void* p)							override;
		virtual void	Reset		()									override;

		const bool IsFull() override
		{
			if (this->m_MemorySize == this->m_MemoryUsed)
			{
				return true;
			}

			return false;
		}
	};

}}}