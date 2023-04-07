#pragma once

#include "Engine/API.h"
#include "ECS/ECS.h"
#include "ECS/ComponentFilter.h"

namespace FlatFormer { namespace ECS {

	class ISystem
	{
		friend class SystemManager;

	private:
		std::vector<EntityID> m_SystemEntites;

	protected:
		ComponentFilter m_SystemFilter;

	public:
		ISystem();
		virtual ~ISystem();

		virtual inline const SystemTypeID GetStaticSystemTypeID() const = 0;
		virtual inline const char* GetSystemTypeName() const = 0;

		virtual void Initialize() = 0;
		virtual void PreUpdate(float32 deltaTime) = 0;
		virtual void Update(float32 deltaTime) = 0;
		virtual void PostUpdate(float32 deltaTime) = 0;

		const std::vector<EntityID>& GetEntities() const;
		ComponentFilter GetFilter() const;

	protected:
		virtual void OnEntityAdded(EntityID entityID) {};
		virtual void OnEntityRemoved(EntityID entityID) {};

	private:
		void AddEntity(EntityID entityID);
		void RemoveEntity(EntityID entityID);
	};
	
}}