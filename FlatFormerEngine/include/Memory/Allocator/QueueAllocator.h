#pragma once

#include "Memory/Allocator/IAllocator.h"

namespace FlatFormer { namespace Memory { namespace Allocator {

	class QueueAllocator : public IAllocator
	{
	private:
		FFE_DECLARE_LOGGER

		// информация о блоке
		struct AllocMetaInfo
		{
			void*	next;				// pointer to the nex avaliable block
		};

		void*		lastPtr;			// last allocated memory block
		void*		currentFirstPtr;	// first allocated memory block
		uint16_t	lastMemBlockSize;   // last block size
		uintptr_t	endOfMemory;		// end of memory area (service field)

	public:

		QueueAllocator(size_t memSize, const void* mem);

		virtual ~QueueAllocator();

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