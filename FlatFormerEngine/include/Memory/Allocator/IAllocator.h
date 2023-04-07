#pragma once

#include "Engine/API.h"

namespace FlatFormer { namespace Memory { namespace Allocator {

	// Aligns the address contained in the pointer
	// The address is considered to be aligned by "alignment"
	// if it is a multiple of alignment i.e.divisible without a remainder
	static inline void* AlignForward(void* address, uint8_t alignment)
	{
		return (void*)((reinterpret_cast<uintptr_t>(address) + static_cast<uintptr_t>(alignment - 1)) & static_cast<uintptr_t>(~(alignment - 1)));
	}


	// Returns the number of bytes needed to align the address by "alignment"
	static inline uint8_t GetAdjustment(const void* address, uint8_t alignment)
	{
		uint8_t adjustment = alignment - (reinterpret_cast<uintptr_t>(address)& static_cast<uintptr_t>(alignment - 1));

		return adjustment == alignment ? 0 : adjustment;
	}


	// Returns the number of bytes needed to align the address by "alignment"
	// and checks if there is enough memory to fit more "extra" bytes into it
	static inline uint8_t GetAdjustment(const void* address, uint8_t alignment, uint8_t extra)
	{
		uint8_t adjustment = GetAdjustment(address, alignment);

		uint8_t neededSpace = extra;

		if (adjustment < neededSpace)
		{
			neededSpace -= adjustment;

			///Increase adjustment to fit header
			adjustment += alignment * (neededSpace / alignment);

			if (neededSpace % alignment > 0)
				adjustment += alignment;
		}

		return adjustment;
	}


	class IAllocator
	{
	protected:
		const size_t		m_MemorySize;
		const void*			m_MemoryFirstAddress;

		size_t				m_MemoryUsed;
		uint64_t			m_MemoryAllocations;

	public:

		IAllocator (const size_t memSize, const void* mem);

		virtual ~IAllocator ();

		// Allocates a block of size size aligned to "alignment"
		virtual void* Allocate(size_t size, uint8_t alignment) = 0;


		// Allocates a block of size size without alignment
		virtual void* Allocate(size_t size) = 0;


		// Frees up memory
		virtual void Deallocate(void* p) = 0;


		// Resets the state of the allocator to its initial state
		virtual void Reset() = 0;


		// Returns true if there is no free memory, false otherwise
		virtual const bool IsFull() = 0;


		// Returns the total amount of memory
		inline size_t GetMemorySize() const
		{
			return this->m_MemorySize;
		}


		// Returns a pointer to the starting memory address
		inline const void* GetMemoryAddress0() const
		{
			return this->m_MemoryFirstAddress;
		}


		// Returns the amount of memory used
		inline size_t GetUsedMemory() const
		{
			return this->m_MemoryUsed;
		}


		// Returns the number of allocated memory blocks
		inline uint64_t GetAllocationCount() const
		{
			return this->m_MemoryAllocations;
		}
	};

}}}