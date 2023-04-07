#pragma once

#include "Engine/API.h"
#include "Engine/Service/IDPoolUser.h"

#include "ECS/IComponent.h"
#include "ECS/ComponentFilter.h"

#include "Memory/ObjectDispatcher.h"

#include "Util/TypeIDCounter.h"

namespace FlatFormer { namespace ECS {

	class ComponentManager : public Service::FFE_SERV_USER_IDPool
	{
#pragma region Using
		using ComponentMap = std::unordered_map<typename ComponentID::value_type, IComponent*>;
		using EntityComponentMap = std::unordered_map<typename EntityID::value_type, std::vector<ComponentID>>;
		using EntityComponentFilterMap = std::unordered_map<typename EntityID::value_type, ComponentFilter>;
		using ComponentDispatcherRegistry = std::vector<Memory::IObjectDispatcher*>;
#pragma endregion

	private:
		ComponentMap				m_ComponentMap;
		EntityComponentMap			m_EntityComponentMap;
		EntityComponentFilterMap	m_EntityComponentFilterMap;
		ComponentDispatcherRegistry	m_ComponentDispatcherRegistry;

		FFE_DECLARE_LOGGER

		ComponentManager(const ComponentManager&) = delete;
		ComponentManager& operator=(ComponentManager&) = delete;

	public:
		ComponentManager();
		~ComponentManager();


		// Create a ComponentType for an entity with the given entityID.
		// On successful creation, an object is returned, otherwise nullptr.
		template<class ComponentType, class ...ARGS>
		ComponentType* AddComponent(const EntityID entityID, ARGS&&... args)
		{
			ComponentTypeID CTID = ComponentType::STATIC_TYPE_ID;

			/// memory request to create a new component
			void* componentPtr = GetComponentDispatcher<ComponentType>()->Place();

			/// adding a new component if the memory for the ComponentType object was allocated successfully
			if (componentPtr != nullptr)
			{
				/// request an ID for the component
				ComponentID componentID = this->GetIDPoolService().AcquireID();

				/// constructing the component "in place"
				ComponentType* component = new (componentPtr)ComponentType(std::forward<ARGS>(args)...);
				component->m_ComponentID = componentID;
				component->m_OwnerID = entityID;

				std::pair<ComponentID, IComponent*> keyValuePair(componentID, component);
				m_ComponentMap.insert(keyValuePair);

				std::vector<ComponentID> vec (Util::TypeIDCounter<IComponent>::GetCount(), INVALID_COMPONENT_ID);
				std::pair<EntityID, std::vector<ComponentID>> keyValuePair2(entityID, vec);
				m_EntityComponentMap.insert(keyValuePair2);
				m_EntityComponentMap[entityID][CTID] = componentID;

				m_EntityComponentFilterMap[entityID];
				m_EntityComponentFilterMap[entityID].SetItemByIndex(CTID, true);

				return component;
			}
			else
			{
				FFE_LOG_WARNING("Unable to create component '{}'", typeid(ComponentType).name());
				return nullptr;
			}
		}


		// Removes the ComponentType from the entity with the given entityID
		template<class ComponentType>
		void RemoveComponent(EntityID entityID)
		{
			ComponentTypeID CTID = ComponentType::STATIC_TYPE_ID;
			ComponentID componentID = m_EntityComponentMap[entityID][CTID];
			IComponent* component = m_ComponentMap[componentID];

			assert(component != nullptr && "ASSERT: Trying to remove a component wich is not used by this entity!");

			m_ComponentMap.erase(componentID);
			component->~IComponent();
			GetComponentDispatcher<ComponentType>()->Replace(component);
			this->GetIDPoolService().ReleaseID(componentID);
			m_EntityComponentMap[entityID][CTID] = INVALID_COMPONENT_ID;
			m_EntityComponentFilterMap[entityID].SetItemByIndex(CTID, false);
		}


		// Removes all components from the entity with the given entityID
		void RemoveAllComponents(EntityID entityID)
		{
			const size_t NUM_COMPONENTS = m_EntityComponentMap[entityID].size();
			for (ComponentTypeID CTID = 0; CTID < NUM_COMPONENTS; CTID++)
			{
				ComponentID componentID = m_EntityComponentMap[entityID][CTID];
				if (componentID == INVALID_COMPONENT_ID)
					continue;

				IComponent* component = m_ComponentMap[componentID];
				if (component != nullptr)
				{
					m_ComponentMap.erase(componentID);
					component->~IComponent();
					m_ComponentDispatcherRegistry[CTID]->Replace(component);
					this->GetIDPoolService().ReleaseID(componentID);
					m_EntityComponentMap[entityID][CTID] = INVALID_COMPONENT_ID;
					m_EntityComponentFilterMap[entityID].SetItemByIndex(CTID, false);
				}
			}
		}


		bool IsEqualEntityFilter(EntityID entityID, const ComponentFilter& filter)
		{
			return m_EntityComponentFilterMap[entityID].IsContain(filter);
		}


		// Get the ComponentType of the entity with the given entityID.
		// If there is no component, nullptr is returned.
		template<class ComponentType>
		ComponentType* GetComponent(EntityID entityID)
		{
			ComponentTypeID CTID = ComponentType::STATIC_TYPE_ID;

			auto test = m_EntityComponentMap[entityID];
			ComponentID componentID = m_EntityComponentMap[entityID][CTID];

			if (componentID == INVALID_COMPONENT_ID)
				return nullptr;

			return static_cast<ComponentType*>(m_ComponentMap[componentID]);
		}

		template<class ComponentType>
		inline bool HasComponent(EntityID entityID)
		{
			ComponentTypeID CTID = ComponentType::STATIC_TYPE_ID;
			ComponentID componentID = m_EntityComponentMap[entityID][CTID];

			if (componentID == INVALID_COMPONENT_ID)
				return false;
			return true;
		}

		inline IComponent* GetComponentByID(ComponentID componentID)
		{
			return m_ComponentMap[componentID];
		}


		// Get all the beans of the EntityType.
		// If entities of the given type exist,
		// then a list is returned, otherwise nullptr
		template<class ComponentType>
		const std::vector<ComponentType*>& GetComponentsOfType()
		{
			auto objectDispatcher = (Memory::ObjectDispatcher<ComponentType>*)GetComponentDispatcher<ComponentType>();
			return objectDispatcher->GetObjects();
		}


	private:
		// Get the dispatcher responsible for the ComponentType,
		// if the dispatcher did not exist, then it is created
		template<class ComponentType>
		Memory::IObjectDispatcher* GetComponentDispatcher()
		{
			ComponentTypeID CTID = ComponentType::STATIC_TYPE_ID;

			if (m_ComponentDispatcherRegistry[CTID] == nullptr)
			{
				m_ComponentDispatcherRegistry[CTID] = new Memory::ObjectDispatcher<ComponentType>("_ObjectDispatcher");
			}

			return m_ComponentDispatcherRegistry[CTID];
		}

	};

}}