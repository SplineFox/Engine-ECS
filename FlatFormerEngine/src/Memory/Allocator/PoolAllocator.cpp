#include "Memory/Allocator/PoolAllocator.h"

namespace FlatFormer { namespace Memory { namespace Allocator {

	PoolAllocator::PoolAllocator(size_t memSize, const void* mem, size_t blockSize, uint8_t alignment) :
		IAllocator(memSize, mem),
		BLOCK_ALIGNMENT(alignment)
	{
		if (blockSize < sizeof(void*))
		{
			this->BLOCK_SIZE = sizeof(void*);
		}
		else
		{
			this->BLOCK_SIZE = blockSize;
		}

		FFE_DEFINE_LOGGER("PoolAllocator")
		FFE_LOG_DEBUG("Initialized")
		FFE_LOG_DEBUG("Min size of memory block = {}", this->BLOCK_SIZE)

		this->expandedMemSize = 0;
		this->Reset();
	}


	PoolAllocator::~PoolAllocator()
	{
		this->freeMemoryList = nullptr;

		FFE_LOG_DEBUG("Released")
	}


	/// there is no need for the PoolAllocator to use the aligned memory allocation function each time,
	/// because it is aligned at the beginning and is always allocated with alignment
	void* PoolAllocator::Allocate(size_t size, uint8_t alignment)
	{
		assert(true && "PoolAllocator does not maintain \"allocate(size_t size, uint8_t alignment)\", use the \"allocate(size_t size)\" method");
		return nullptr;
	}


	void* PoolAllocator::Allocate(size_t size)
	{
		if (this->freeMemoryList == nullptr)
		{
			FFE_LOG_DEBUG("Not enought memory!")
			return nullptr;
		}

		void* freePtr = this->freeMemoryList;

		this->freeMemoryList = (void**)(*this->freeMemoryList);

		this->m_MemoryUsed += this->BLOCK_SIZE;
		this->m_MemoryAllocations++;

		return freePtr;
	}


	void PoolAllocator::Deallocate(void* mem)
	{
		if (mem == nullptr)
		{
			FFE_LOG_ERROR("Pointer is nullptr! Releasing is impossible")
			return;
		}

		*((void**)mem) = this->freeMemoryList;

		this->freeMemoryList = (void**)mem;

		this->m_MemoryUsed -= this->BLOCK_SIZE;
		this->m_MemoryAllocations--;
	}


	bool PoolAllocator::IsExpandable(size_t memSize, const void* mem)
	{
		uint8_t adjustment = GetAdjustment(mem, this->BLOCK_ALIGNMENT);

		size_t numObjects = (size_t)(memSize - adjustment) / this->BLOCK_SIZE;

		if (numObjects == 0)
		{
			//TODO debug
			return false;
		}

		return true;
	}


	void PoolAllocator::Expand(size_t memSize, const void* mem)
	{
		if (memSize < this->BLOCK_SIZE )
		{
			FFE_LOG_DEBUG("Min mem size [memSize= {}] must  be >= block size [blockSize= {}]", memSize, this->BLOCK_SIZE)
			return;
		}

		uint8_t adjustment = GetAdjustment(mem, this->BLOCK_ALIGNMENT);

		size_t numObjects = (size_t)(memSize - adjustment) / this->BLOCK_SIZE;

		void* p = (void*)((uintptr_t)mem + adjustment);

		for (size_t i = 0; i < numObjects; ++i)
		{
			*((void**)p) = this->freeMemoryList;
			this->freeMemoryList = (void**)p;

			p = (void*)((uintptr_t)p + this->BLOCK_SIZE);
		}

		this->expandedMem.push_back(ExpandedMemInfo((void*)mem, memSize));
		this->expandedMemSize += memSize;

		FFE_LOG_DEBUG("Memory expanded on {} bytes", memSize)
	}


	void PoolAllocator::Reset()
	{
		this->freeMemoryList = nullptr;

		uint8_t adjustment = GetAdjustment(this->m_MemoryFirstAddress, this->BLOCK_ALIGNMENT);

		int numObjects = (int)(this->m_MemorySize - adjustment) / this->BLOCK_SIZE;

		if (numObjects > 0)
		{
			union
			{
				void*		asVoidPtr;
				uintptr_t	asUptr;
			};

			asVoidPtr = (void*)this->m_MemoryFirstAddress;

			asUptr += adjustment;

			this->freeMemoryList = (void**)asVoidPtr;

			void** ptr = this->freeMemoryList;

			for (int i = 0; i < (numObjects - 1); ++i)
			{
				*ptr = (void*)((uintptr_t)ptr + this->BLOCK_SIZE);

				ptr = (void**)*ptr;
			}

			*ptr = nullptr;
		}

		void*	expPtr;

		for (auto item : this->expandedMem)
		{
			adjustment = GetAdjustment(item.ptr, this->BLOCK_ALIGNMENT);

			numObjects = (size_t)(item.memSize - adjustment) / this->BLOCK_SIZE;

			expPtr = (void*)((uintptr_t)item.ptr + adjustment);

			for (size_t i = 0; i < numObjects; ++i)
			{
				*((void**)expPtr) = this->freeMemoryList;
				this->freeMemoryList = (void**)expPtr;

				expPtr = (void*)((uintptr_t)expPtr + this->BLOCK_SIZE);
			}
		}
		FFE_LOG_DEBUG("Reseted")
	}

}}}
