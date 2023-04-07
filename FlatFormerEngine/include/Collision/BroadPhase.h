#pragma once
#include "Collision/SeparationGrid.h"
#include "Engine/API.h"

namespace FlatFormer {namespace Collision {

	class BroadPhase 
	{
	#pragma region Using
			using ColliderAndId = std::pair<ColliderComponent*, ECS::EntityID>;
			using ColliderVector = std::vector<ColliderAndId*>;
			using ColliderAndIdPair = std::pair<ColliderAndId*, ColliderAndId*>;
			using ColliderPairSet = std::set<ColliderAndIdPair>;
			using AABBVector = std::vector<FF_AABB>;
	#pragma endregion

	private :

		SeparationGrid*		m_grid;	
		ColliderVector		m_colliders;		// list of pairs collider - entity id
		AABBVector			m_oldAABBs;			// list of aabb to check
		ColliderPairSet		m_broadColliders;	// set of colliders intersecting in phase

		void UdateGrid();

	public:

		BroadPhase();
		~BroadPhase();

		void AddToPhase(ColliderAndId* userData);

		void DeleteFromPhase(ColliderAndId* userData);

		void Update();

		ColliderPairSet::const_iterator begin()
		{
			return m_broadColliders.cbegin();
		}

		ColliderPairSet::const_iterator end()
		{
			return m_broadColliders.cend();
		}
	};
}}