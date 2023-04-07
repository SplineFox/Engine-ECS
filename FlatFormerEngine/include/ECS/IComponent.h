#pragma once

#include "Engine/API.h"
#include "ECS/ECS.h"

namespace FlatFormer { namespace ECS {

	class IComponent
	{
		friend class ComponentManager;

	protected:
		ComponentID m_ComponentID;
		EntityID	m_OwnerID;
		bool		m_IsActive;

	public:
		IComponent();
		virtual ~IComponent();

		inline const ComponentID GetComponentID() const { return this->m_ComponentID; }
		inline const EntityID GetOwnerID() const { return this->m_OwnerID; }

		inline void SetActive(bool state) { this->m_IsActive = state; }
		inline bool IsActive() const { return this->m_IsActive; }

		inline const bool operator==(const IComponent& other) const { return this->m_ComponentID == other.m_ComponentID; }
		inline const bool operator!=(const IComponent& other) const { return this->m_ComponentID == other.m_ComponentID; }
	};
	
}}
