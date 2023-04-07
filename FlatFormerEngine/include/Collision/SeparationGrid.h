#pragma once

#include "ECS/Collision/Colliders/ColliderComponent.h"

#include "Engine/API.h"
#include "Engine/Platform.h"
#include "Engine/Service/MemoryManagerUser.h"

#include "Collision/FF_AABB.h"
#include "Collision/QuadTree/QuadTree.h"

#include "Memory/Allocator/PoolAllocator.h"


#include "Engine/Service/ComponentManagerUser.h"
#include "Engine/Service/EntityManagerUser.h"
#include "ECS/Collision/QuadTreeDebugComponent.h"

namespace FlatFormer { namespace Collision {

	class SeparationGrid : 
		public Service::FFE_SERV_USER_MemoryManager,
		public Service::FFE_SERV_USER_ComponentManager,
		public Service::FFE_SERV_USER_EntityManager
	{
	#pragma region Using
		using BoundingBox = FF_AABB;
		using UserData = std::pair<ColliderComponent*, ECS::EntityID >;
		using PoolAllocator = FlatFormer::Memory::Allocator::PoolAllocator;
		using GridMap = std::unordered_map<vec2, QuadTree*, vec2::hash<vec2>>;
	#pragma endregion

	private:
		GridMap			m_cellsMap;
		size_t			m_cellSizeX		= 0;
		size_t			m_cellSizeY		= 0;
		PoolAllocator*	m_treeAllocator = nullptr;
		PoolAllocator*	m_nodeAllocator = nullptr;

	public:

		SeparationGrid(size_t cellSizeX, size_t cellSizeY);
		~SeparationGrid();

		void AddToGrid(const FF_AABB& aabb, UserData* userData);

		void RemoveFromGrid(const FF_AABB& aabb);

		// populates the list to check for intersection
		void FillUserDataList(const FF_AABB& bound, std::vector<UserData*>& outList);

		
		void Update();

		void Clear();

		inline size_t GetCellWidth()
		{
			return m_cellSizeX;
		}

		inline size_t GetCellHeight()
		{
			return m_cellSizeY;
		}
	};
}}