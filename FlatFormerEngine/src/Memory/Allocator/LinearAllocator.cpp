#include "Memory/Allocator/LinearAllocator.h"

namespace FlatFormer { namespace Memory { namespace Allocator {

	LinearAllocator::LinearAllocator(size_t memSize, const void* mem) :
		IAllocator(memSize, mem)
	{
		FFE_DEFINE_LOGGER("LinearAllocator")
		FFE_LOG_DEBUG("Initialized")
	}


	LinearAllocator::~LinearAllocator()
	{
		this->Reset();
		FFE_LOG_DEBUG("Released")
	}


	void* LinearAllocator::Allocate(size_t memSize, uint8_t alignment)
	{
		/// the void* type does not allow arithmetic operations with it, such as
		/// void *p = malloc(8); p+= 2;
		/// therefore, for convenience, we use union and perform all actions with uintptr_t
		union
		{
			void*		asVoidPtr;
			uintptr_t	asUptr;
		};

		asVoidPtr = (void*)this->m_MemoryFirstAddress;
		asUptr += this->m_MemoryUsed;

		/// get the required number of bytes for alignment
		uint8_t adjustment = GetAdjustment(asVoidPtr, alignment);

		/// check if there is enough memory to allocate
		if (this->m_MemoryUsed + memSize + adjustment > this->m_MemorySize)
		{
			FFE_LOG_DEBUG("Not enought memory!")
			return nullptr;
		}

		asUptr += adjustment;
		this->m_MemoryUsed += memSize + adjustment;
		this->m_MemoryAllocations++;

		return asVoidPtr;
	}


	void* LinearAllocator::Allocate(size_t memSize)
	{
		/// the void* type does not allow arithmetic operations with it, such as
		/// void *p = malloc(8); p+= 2;
		/// therefore, for convenience, we use union and perform all actions with uintptr_t
		union
		{
			void*		asVoidPtr;
			uintptr_t	asUptr;
		};

		asVoidPtr = (void*)this->m_MemoryFirstAddress;
		asUptr += this->m_MemoryUsed;

		/// check if there is enough memory to allocate
		if (this->m_MemoryUsed + memSize > this->m_MemorySize)
		{
			FFE_LOG_DEBUG("Not enought memory!")
			return nullptr;
		}

		this->m_MemoryUsed += memSize;
		this->m_MemoryAllocations++;

		return asVoidPtr;
	}


	void LinearAllocator::Deallocate(void* mem)
	{
		assert(true && "LinearAllocator does not maintain \"deallocate()\", use the \"reset()\" method");
	}


	void LinearAllocator::Reset()
	{
		this->m_MemoryUsed = 0;
		this->m_MemoryAllocations = 0;
		FFE_LOG_DEBUG("Reseted")
	}

}}}