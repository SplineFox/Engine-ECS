#pragma once

#include "Engine/API.h"
#include "Engine/Service/IDPoolUser.h"
#include "Engine/Service/ComponentManagerUser.h"
#include "Engine/Service/SystemManagerUser.h"

#include "Memory/ObjectDispatcher.h"

namespace FlatFormer { namespace ECS {

	class EntityManager :  public Service::FFE_SERV_USER_ComponentManager,
						   public Service::FFE_SERV_USER_SystemManager,
						   public Service::FFE_SERV_USER_IDPool
	{
#pragma region Using
		using EntityVector = std::vector<EntityID>;
		using PendingDestroyedEntities = EntityVector;
#pragma endregion


	private:
		EntityVector m_ActiveEntities;
		EntityVector m_InactiveEntities;
		PendingDestroyedEntities m_PendingDestroyedEntities;

		FFE_DECLARE_LOGGER

		EntityManager(const EntityManager&) = delete;
		EntityManager& operator=(EntityManager&) = delete;

	public:
		EntityManager();
		~EntityManager();


		EntityID CreateEntity()
		{
			EntityID entityID = this->GetIDPoolService().AcquireID();
			m_ActiveEntities.emplace_back(entityID);
			this->GetSystemManagerService().OnEntityCreated(entityID);

			return entityID;
		}

		template<class ComponentType, class ...ARGS>
		EntityID CreateEntityWith(ARGS&&... args)
		{
			EntityID entityID = CreateEntity();
			this->GetComponentManagerService().AddComponent<ComponentType>(entityID, std::forward<ARGS>(args)...);

			return entityID;
		}


		void DestroyEnity(EntityID entityID)
		{
			EntityVector::iterator iterator = std::find(m_ActiveEntities.begin(), m_ActiveEntities.end(), entityID);
			if (iterator != m_ActiveEntities.end())
			{
				m_ActiveEntities.erase(iterator);
				m_PendingDestroyedEntities.emplace_back(entityID);
				this->GetSystemManagerService().OnEntityDestroyed(entityID);
				return;
			}

			iterator = std::find(m_InactiveEntities.begin(), m_InactiveEntities.end(), entityID);
			if (iterator != m_InactiveEntities.end())
			{
				m_InactiveEntities.erase(iterator);
				m_PendingDestroyedEntities.emplace_back(entityID);
				this->GetSystemManagerService().OnEntityDestroyed(entityID);
				return;
			}
		}


		void RemoveDestroyedEntities()
		{
			for (int i = 0; i < m_PendingDestroyedEntities.size(); i++)
			{
				this->GetComponentManagerService().RemoveAllComponents(m_PendingDestroyedEntities[i]);
				this->GetIDPoolService().ReleaseID(m_PendingDestroyedEntities[i]);
			}
			m_PendingDestroyedEntities.clear();
		}


		inline void ActivateEntity(EntityID entityID)
		{
			EntityVector::iterator iterator = std::find(m_InactiveEntities.begin(), m_InactiveEntities.end(), entityID);
			if (iterator != m_InactiveEntities.end())
			{
				m_InactiveEntities.erase(iterator);
				m_ActiveEntities.emplace_back(entityID);
				this->GetSystemManagerService().OnEntityActivated(entityID);
			}
		}


		inline void DeactivateEntity(EntityID entityID)
		{
			EntityVector::iterator iterator = std::find(m_ActiveEntities.begin(), m_ActiveEntities.end(), entityID);
			if (iterator != m_ActiveEntities.end())
			{
				m_ActiveEntities.erase(iterator);
				m_InactiveEntities.emplace_back(entityID);
				this->GetSystemManagerService().OnEntityDeactvated(entityID);
			}
		}


		inline bool IsActive(EntityID entityID)
		{
			EntityVector::iterator iterator = std::find(m_ActiveEntities.begin(), m_ActiveEntities.end(), entityID);
			if (iterator != m_ActiveEntities.end())
				return true;

			return false;
		}


		// Called by Engine
		inline void Update()
		{
			this->GetSystemManagerService().OnUpdateSystemEntities(m_ActiveEntities);
		}
	};

}}

