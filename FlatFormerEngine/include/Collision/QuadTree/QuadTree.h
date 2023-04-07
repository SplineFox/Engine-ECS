#pragma once

#include "Collision/QuadTree/QuadTreeNode.h"

#include "Memory/Allocator/PoolAllocator.h"

namespace FlatFormer { namespace Collision {

	class QuadTree : public Service::FFE_SERV_USER_MemoryManager
	{
#pragma region Using
		using BoundingBox = Collision::FF_AABB;
		using UserData = std::pair<ColliderComponent*, ECS::EntityID >;
		using PoolAllocator = FlatFormer::Memory::Allocator::PoolAllocator;
#pragma endregion

	private:

		QuadTreeNode*	m_rootNode		= nullptr;
		PoolAllocator*	m_nodeAllocator	= nullptr;

	public:

		QuadTree(BoundingBox boundary, int capacity, PoolAllocator*	nodeAllocator);
		~QuadTree();


		void Insert(const FF_AABB& aabb, UserData* userData);


		void Remove(const FF_AABB& aabb);


		void FillUserDataList(const FF_AABB& bound, std::vector<UserData*>& outList);


		void FillDebug(std::vector<FF_AABB>& outList);


		inline bool IsEmpty()
		{
			return m_rootNode->GetCount() == 0;
		}


		void Clear();
	};
}}