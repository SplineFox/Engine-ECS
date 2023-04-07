#pragma once

#include "Engine/API.h"
#include "Engine/Service/MemoryManagerUser.h"
#include "Engine/Service/ComponentManagerUser.h"

#include "ECS/ISystem.h"

#include "Util/TypeIDCounter.h"

#include "Memory/Allocator/LinearAllocator.h"

namespace FlatFormer { namespace ECS {

	class SystemManager : public Service::FFE_SERV_USER_MemoryManager,
						  public Service::FFE_SERV_USER_ComponentManager
	{
#pragma region Using
		using SystemVector = std::vector<ISystem*>;
		using SystemAllocator = Memory::Allocator::LinearAllocator;
		using EntitySystemMap = std::unordered_map<typename EntityID::value_type, std::vector<bool>>;
#pragma endregion

	
	private:
		SystemVector		m_Systems;
		SystemAllocator*	m_SystemAllocator;
		EntitySystemMap		m_EntitySystemMap;

		FFE_DECLARE_LOGGER

		SystemManager(const SystemManager&) = delete;
		SystemManager& operator=(SystemManager&) = delete;

	public:
		SystemManager();
		~SystemManager();


		// Create a system of type SystemType.
		// On success, an object is returned, otherwise nullptr.
		template<class SystemType, class ...ARGS>
		SystemType* AddSystem(ARGS&&... args)
		{
			void* systemPtr = m_SystemAllocator->Allocate(sizeof(SystemType));
			
			if (systemPtr != nullptr)
			{
				SystemType* system = new (systemPtr)SystemType(std::forward<ARGS>(args)...);

				FFE_LOG_TRACE("'{}' system created", typeid(SystemType).name());
				m_Systems.emplace_back(system);

				return system;
			}
			else
			{
				FFE_LOG_WARNING("Unable to create system '{}'", typeid(SystemType).name());
				return nullptr;
			}
		}


		// Called by Engine
		void Update(float32 deltaTime)
		{
			for (auto system : m_Systems)
			{
				system->PreUpdate(deltaTime);
			}

			for (auto system : m_Systems)
			{
				system->Update(deltaTime);
			}

			for (auto system : m_Systems)
			{
				system->PostUpdate(deltaTime);
			}
		}


		// Called by EntityManager
		void OnEntityCreated(EntityID entityID)
		{
			m_EntitySystemMap[entityID] = std::vector<bool>(32, false);
		}


		// Called by EntityManager
		void OnEntityActivated(EntityID entityID)
		{
			auto& ComponentManager = this->GetComponentManagerService();
			EntitySystemMap::iterator iterator;
			for (int j = 0; j < m_Systems.size(); j++)
			{
				iterator = m_EntitySystemMap.find(entityID);
				if (ComponentManager.IsEqualEntityFilter(entityID, m_Systems[j]->GetFilter()))
				{
					m_Systems[j]->AddEntity(entityID);
					iterator->second[j] = true;
				}
			}
		}


		// Called by EntityManager
		void OnEntityDestroyed(EntityID entityID)
		{
			EntitySystemMap::iterator iterator;
			for (int j = 0; j < m_Systems.size(); j++)
			{
				iterator = m_EntitySystemMap.find(entityID);
				if (iterator->second[j])
				{
					m_Systems[j]->RemoveEntity(entityID);
				}
			}
			m_EntitySystemMap.erase(entityID);
		}


		// Called by EntityManager
		void OnEntityDeactvated(EntityID entityID)
		{
			auto& ComponentManager = this->GetComponentManagerService();
			EntitySystemMap::iterator iterator;
			for (int j = 0; j < m_Systems.size(); j++)
			{
				iterator = m_EntitySystemMap.find(entityID);
				if (ComponentManager.IsEqualEntityFilter(entityID, m_Systems[j]->GetFilter()))
				{
					m_Systems[j]->RemoveEntity(entityID);
					iterator->second[j] = false;
				}
			}
		}


		void OnUpdateSystemEntities(const std::vector<EntityID>& activeEntites)
		{
			auto& ComponentManager = this->GetComponentManagerService();
			EntitySystemMap::iterator iterator;

			for (int i = 0; i < activeEntites.size(); i++)
			{
				for (int j = 0; j < m_Systems.size(); j++)
				{
					iterator = m_EntitySystemMap.find(activeEntites[i]);
					if (ComponentManager.IsEqualEntityFilter(activeEntites[i], m_Systems[j]->GetFilter()))
					{
						if (!iterator->second[j])
						{
							m_Systems[j]->AddEntity(activeEntites[i]);
							iterator->second[j] = true;
						}
					}
					else
					{
						if (iterator->second[j])
						{
							m_Systems[j]->RemoveEntity(activeEntites[i]);
							iterator->second[j] = false;
						}
					}
				}
			}
		}

	};

}}

