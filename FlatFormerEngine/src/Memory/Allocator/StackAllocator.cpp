#include "Memory/Allocator/StackAllocator.h"

namespace FlatFormer { namespace Memory { namespace Allocator {

	StackAllocator::StackAllocator(size_t memSize, const void* mem) :
		IAllocator(memSize, mem)
	{
		FFE_DEFINE_LOGGER("StackAllocator")
		FFE_LOG_DEBUG("Initialized")
	}


	StackAllocator::~StackAllocator()
	{
		this->Reset();
		FFE_LOG_DEBUG("Released")
	}


	void* StackAllocator::Allocate(size_t memSize, uint8_t alignment)
	{
		union
		{
			void*			asVoidPtr;
			uintptr_t		asUptr;
			AllocMetaInfo*	asMeta;
		};

		asVoidPtr = (void*)this->m_MemoryFirstAddress;

		/// current top adress (top of the stack)
		asUptr += this->m_MemoryUsed;
		/// get alignment to save meta information
		uint8_t adjustment = GetAdjustment(asVoidPtr, alignment, sizeof(AllocMetaInfo));

		/// check if there is enough memory
		if (this->m_MemoryUsed + memSize + adjustment > this->m_MemorySize)
		{
			FFE_LOG_DEBUG("Not enought memory!")
			return nullptr;
		}

		/// move the address to align it
		asUptr += adjustment;

		/// store information about the block
		(asMeta-1)->memBlockSize = memSize + adjustment;

		/// update service information
		this->m_MemoryUsed += memSize + adjustment;
		this->m_MemoryAllocations++;

		/// return aligned memory address
		return asVoidPtr;
	}


	void* StackAllocator::Allocate(size_t memSize)
	{
		union
		{
			void*			asVoidPtr;
			uintptr_t		asUptr;
			AllocMetaInfo*	asMeta;
		};

		asVoidPtr = (void*)this->m_MemoryFirstAddress;
		/// current top adress (top of the stack)
		asUptr += this->m_MemoryUsed;
		/// get alignment to save meta information
		uint8_t adjustment = GetAdjustment(asVoidPtr, 1, sizeof(AllocMetaInfo));

		/// check if there is enough memory
		if (this->m_MemoryUsed + memSize + adjustment > this->m_MemorySize)
		{
			FFE_LOG_DEBUG("Not enought memory!")
			return nullptr;
		}

		/// move the address to align it
		asUptr += adjustment;
		/// store information about the block
		(asMeta - 1)->memBlockSize = memSize + adjustment;
		/// update service information
		this->m_MemoryUsed += memSize + adjustment;
		this->m_MemoryAllocations++;

		/// return aligned memory address
		return asVoidPtr;
	}


	void StackAllocator::Deallocate(void* mem)
	{
		if (mem == nullptr)
		{
			FFE_LOG_ERROR("Pointer is nullptr! Releasing is impossible")
			return;
		}

		union
		{
			void*			asVoidPtr;
			uintptr_t		asUptr;
			AllocMetaInfo*	asMeta;
		};

		asVoidPtr = mem;

		this->m_MemoryUsed -= (asMeta-1)->memBlockSize;
		this->m_MemoryAllocations--;
	}


	void StackAllocator::Reset()
	{
		this->m_MemoryUsed = 0;
		this->m_MemoryAllocations = 0;

		FFE_LOG_DEBUG("Reseted")
	}

}}}