#include "Memory/Allocator/QueueAllocator.h"

namespace FlatFormer { namespace Memory { namespace Allocator {

	QueueAllocator::QueueAllocator(size_t memSize, const void* mem) :
		IAllocator(memSize, mem)
	{
		FFE_DEFINE_LOGGER("QueueAllocator")

		this->lastPtr = (void*)this->m_MemoryFirstAddress;
		this->currentFirstPtr = (void*)this->m_MemoryFirstAddress;
		this->endOfMemory = (uintptr_t)this->m_MemoryFirstAddress + this->m_MemorySize;
		this->lastMemBlockSize = 0;

		FFE_LOG_DEBUG("Initialized")
	}


	QueueAllocator::~QueueAllocator()
	{
		this->Reset();
		FFE_LOG_DEBUG("Released")
	}


	void* QueueAllocator::Allocate(size_t memSize, uint8_t alignment)
	{
		union
		{
			void*			asVoidPtr;
			uintptr_t		asUptr;
			AllocMetaInfo*	asMeta;
		};

		/// end of queue
		asVoidPtr = this->lastPtr;
		/// get alignment to save meta information
		uint8_t adjustment = GetAdjustment(asVoidPtr, alignment, sizeof(AllocMetaInfo));

		/// variable for calculating the offset in case
		/// there was not enough memory at the end of the area:
		/// 					   [111111111]
		/// 00000000000[11111][1111]000000|
		/// 					   ^offset^
		uint16_t padding = 0;

		if (asVoidPtr >= this->currentFirstPtr && memSize + adjustment > this->endOfMemory - asUptr)
		{
			asVoidPtr = (void*)this->m_MemoryFirstAddress;
			adjustment = GetAdjustment(asVoidPtr, alignment, sizeof(AllocMetaInfo));

			if (this->m_MemoryAllocations > 0)
			{
				padding = (this->endOfMemory - (uintptr_t)this->lastPtr);
				memSize += padding;
			}
		}

		if (this->m_MemoryUsed + memSize + adjustment > this->m_MemorySize)
		{
			FFE_LOG_DEBUG("Not enought memory!")
			return nullptr;
		}

		if (this->m_MemoryAllocations > 0)
		{
			((AllocMetaInfo*)((uintptr_t)this->lastPtr - this->lastMemBlockSize))->next = asVoidPtr;
		}
		else
		{
			this->currentFirstPtr = asVoidPtr;
		}

		this->lastPtr = (void*)((uintptr_t)asVoidPtr + memSize + adjustment - padding);
		(asMeta)->next = this->lastPtr;

		/// move the address to align it
		asUptr += adjustment;

		/// update service information
		this->m_MemoryUsed += memSize + adjustment;
		this->lastMemBlockSize = memSize - padding + adjustment;
		this->m_MemoryAllocations++;

		return asVoidPtr;
	}


	void* QueueAllocator::Allocate(size_t memSize)
	{
		union
		{
			void*			asVoidPtr;
			uintptr_t		asUptr;
			AllocMetaInfo*	asMeta;
		};

		/// end of queue
		asVoidPtr = this->lastPtr;

		/// get alignment to save meta information
		uint8_t adjustment = sizeof(AllocMetaInfo);

		/// variable for calculating the offset in case
		/// there was not enough memory at the end of the area:
		/// 					   [111111111]
		/// 00000000000[11111][1111]000000|
		/// 					   ^offset^
		uint16_t padding = 0;

		if (asVoidPtr >= this->currentFirstPtr && memSize + adjustment > this->endOfMemory - asUptr)
		{
			asVoidPtr = (void*)this->m_MemoryFirstAddress;

			if (this->m_MemoryAllocations > 0)
			{
				padding = (this->endOfMemory - (uintptr_t)this->lastPtr);
				memSize += padding;
			}
		}

		if (this->m_MemoryUsed + memSize + adjustment > this->m_MemorySize)
		{
			FFE_LOG_DEBUG("Not enought memory!")
			return nullptr;
		}

		if (this->m_MemoryAllocations > 0)
		{
			((AllocMetaInfo*)((uintptr_t)this->lastPtr - this->lastMemBlockSize))->next = asVoidPtr;
		}
		else
		{
			this->currentFirstPtr = asVoidPtr;
		}

		this->lastPtr = (void*)((uintptr_t)asVoidPtr + memSize + adjustment - padding);
		(asMeta)->next = this->lastPtr;

		/// move the address to align it
		asUptr += adjustment;

		/// update service information
		this->m_MemoryUsed += memSize + adjustment;
		this->lastMemBlockSize = memSize - padding + adjustment;
		this->m_MemoryAllocations++;

		return asVoidPtr;
	}


	void QueueAllocator::Deallocate(void* mem)
	{
		if (mem == nullptr)
		{
			FFE_LOG_ERROR("Pointer is nullptr! Releasing is impossible")
			return;
		}

		this->currentFirstPtr = ((AllocMetaInfo*)this->currentFirstPtr)->next;

		if (this->currentFirstPtr <= this->lastPtr)
		{
			this->m_MemoryUsed = ((uintptr_t)this->lastPtr - (uintptr_t)this->currentFirstPtr);
		}
		else
		{
			this->m_MemoryUsed = this->m_MemorySize - ((uintptr_t)this->currentFirstPtr - (uintptr_t)this->lastPtr);
		}

		this->m_MemoryAllocations--;

		if (this->m_MemoryAllocations == 0)
		{
			this->Reset();
		}
	}


	void QueueAllocator::Reset()
	{
		this->m_MemoryUsed = 0;
		this->lastMemBlockSize = 0;
		this->m_MemoryAllocations = 0;

		this->lastPtr = (void*)this->m_MemoryFirstAddress;
		this->currentFirstPtr = (void*)this->m_MemoryFirstAddress;

		FFE_LOG_DEBUG("Reseted")
	}

}}}