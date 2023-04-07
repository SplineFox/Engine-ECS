#include "ECS/ISystem.h"

namespace FlatFormer { namespace ECS {

	ISystem::ISystem()
	{
	}


	ISystem::~ISystem()
	{
	}
	

	const std::vector<EntityID>& ISystem::GetEntities() const
	{
		return m_SystemEntites;
	}


	ComponentFilter ISystem::GetFilter() const
	{
		return m_SystemFilter;
	}


	void ISystem::AddEntity(EntityID entityID)
	{
		m_SystemEntites.push_back(entityID);
		OnEntityAdded(entityID);
	}


	void ISystem::RemoveEntity(EntityID entityID)
	{
		m_SystemEntites.erase(std::remove(m_SystemEntites.begin(), m_SystemEntites.end(), entityID), m_SystemEntites.end());
		OnEntityRemoved(entityID);
	}
}}