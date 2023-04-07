#pragma once

#include "Engine/Service/MemoryManagerUser.h"

#include "Memory/Allocator/PoolAllocator.h"
#include "Memory/IObjectDispatcher.h"

namespace FlatFormer { namespace Memory {

	template<class ObjectType>
	class ObjectDispatcher : public IObjectDispatcher, public Service::FFE_SERV_USER_MemoryManager
	{
	#pragma region Using
		using ObjectVector = std::vector<ObjectType*>;
		using ObjectPool = Allocator::PoolAllocator;
	#pragma endregion

		FFE_DECLARE_LOGGER

	private:
		
		const size_t GROW_ELEMENT_AMOUNT	= GROW_ELEMENT_SIZE;											// number of elements to expand
		const size_t OBJECT_TYPE_SIZE		= sizeof(ObjectType);											// object type size in bytes
		const size_t GROW_BYTE_AMOUNT		= OBJECT_TYPE_SIZE * GROW_ELEMENT_AMOUNT + alignof(ObjectType);	// bytes to expand

		const char*							m_DispatcherName;												// the name of the allocator to register with the MemoryManager
		ObjectPool*							m_Allocator;													// allocator
		ObjectVector						m_Objects;														// list of objects of the same type

	public:	
		
		ObjectDispatcher(const char* name) :
			m_DispatcherName(name)
		{
			m_Allocator = new ObjectPool
			(
				GROW_BYTE_AMOUNT,
				this->GetMemoryManagerService().Allocate(GROW_BYTE_AMOUNT, m_DispatcherName),
				OBJECT_TYPE_SIZE,
				alignof(ObjectType)
			);

			m_Objects.clear();

			FFE_DEFINE_LOGGER(m_DispatcherName)
			FFE_LOG_DEBUG("Initialized")
		}


		virtual ~ObjectDispatcher()
		{
			/// release all memory taken by the allocator from the manager
			void* ptr;
			ptr = (void*)m_Allocator->PopExpandedMem();
			while (ptr != nullptr)
			{
				this->GetMemoryManagerService().Deallocate(ptr);
				ptr = (void*)m_Allocator->PopExpandedMem();
			}
			this->GetMemoryManagerService().Deallocate((void*)m_Allocator->GetMemoryAddress0());
			ptr = nullptr;

			delete m_Allocator;
			m_Allocator = nullptr;

			m_Objects.clear();

			FFE_LOG_DEBUG("Released")
		}


		// Allocate memory for the object
		void* Place() override
		{
			if (m_Allocator->IsFull())
			{
				/// get a block of memory to expand
				void* mem = this->GetMemoryManagerService().Allocate(GROW_BYTE_AMOUNT, m_DispatcherName);

				/// checking if the allocator can be extended with a block of a given size
				if (m_Allocator->IsExpandable(GROW_BYTE_AMOUNT, mem))
				{
					/// expand if possible
					m_Allocator->Expand(GROW_BYTE_AMOUNT, mem);
				}
				else
				{
					/// if not, then return to the memory manager
					this->GetMemoryManagerService().Deallocate(mem);
				}

				mem = nullptr;
			}

			/// allocate memory for the object
			ObjectType * newObject = (ObjectType *)m_Allocator->Allocate(OBJECT_TYPE_SIZE);

			m_Objects.emplace_back(newObject);

			return newObject;
		}


		// Frees the memory occupied by the object
		void Replace(void* object) override
		{
			if (object == nullptr)
			{
				FFE_LOG_WARNING("Replace: object doesn't exists and can't be removed")
				return;
			}

			m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), (ObjectType *)object), m_Objects.end());

			m_Allocator->Deallocate(object);
		}


		inline size_t GetObjectCount() const override
		{
			return m_Objects.size();
		}


		const ObjectVector& GetObjects() const
		{
			return m_Objects;
		}
	};

}}