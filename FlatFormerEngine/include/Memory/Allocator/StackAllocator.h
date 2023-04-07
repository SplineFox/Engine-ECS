#pragma once

#include "Memory/Allocator/IAllocator.h"

/*
	memory is allocated in blocks, each of which stores the following information:
	[__________|b2_data]...[b1_size,b1_adj|b2_data]...[b2_size,b2_adj|b3_data]...
	Each subsequent block stores information about the previous one, such as its size and offset.
*/
namespace FlatFormer { namespace Memory { namespace Allocator {

	class StackAllocator : public IAllocator
	{
	private:

		FFE_DECLARE_LOGGER

		struct AllocMetaInfo
		{
			uint16_t	memBlockSize;
		};

	public:

		StackAllocator(size_t memSize, const void* mem);

		virtual ~StackAllocator();

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