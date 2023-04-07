#pragma once

#include "Memory/Allocator/IAllocator.h"

namespace FlatFormer { namespace Memory { namespace Allocator {

	class PoolAllocator : public IAllocator
	{
		struct  ExpandedMemInfo
		{
			void*	ptr;
			size_t	memSize;

			ExpandedMemInfo(void* mem, size_t size) :
				ptr(mem),
				memSize(size)
			{}

			~ExpandedMemInfo()
			{
				ptr = nullptr;
			}

		};

	private:
		FFE_DECLARE_LOGGER

		size_t							BLOCK_SIZE;
		uint8_t							BLOCK_ALIGNMENT;

		void**							freeMemoryList;		// list of avaliable blocks
		std::vector<ExpandedMemInfo>	expandedMem;		// list of <pointer, size> pairs to extended memory areas
		size_t							expandedMemSize;    // amount of extended memory

	public:

		PoolAllocator(size_t memSize, const void* mem, size_t blockSize, uint8_t alignment = alignof(uint8_t));

		virtual ~PoolAllocator();

		virtual void*	Allocate		(size_t size, uint8_t alignment)	override;
		virtual void*	Allocate		(size_t size)						override;
		virtual void	Deallocate		(void* p)							override;
		virtual void	Reset			()									override;

		virtual void	Expand			(size_t memSize, const void* mem);
		virtual bool	IsExpandable	(size_t memSize, const void* mem);

		const bool IsFull() override
		{
			if (this->freeMemoryList == nullptr)
			{
				return true;
			}

			return false;
		}


		// Returns the node <pointer, amount of memory>,
		// from the array containing information about extended memory, and removes it (node)
		inline const void* PopExpandedMem()
		{
			if (expandedMem.empty())
			{
				return nullptr;
			}

			ExpandedMemInfo expItem = expandedMem.back();

			void* ptr = expItem.ptr;
			this->expandedMemSize -= expItem.memSize;

			expandedMem.pop_back();

			return ptr;
		}
	};

}}}