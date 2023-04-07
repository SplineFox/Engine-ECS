#pragma once

#include "Engine/API.h"
#include "Engine/Platform.h"
#include "Engine/Service/MemoryManagerUser.h"

#include "Math/vec2.h"
#include "Math/Transform2D.h"

#include "Memory/Allocator/IAllocator.h"
#include "Memory/Allocator/PoolAllocator.h"

#include "Collision/FF_AABB.h"
#include "ECS/Collision/Colliders/ColliderComponent.h"

#include "Util/ID.h"

namespace FlatFormer { namespace Collision {

	struct QuadTreeNodeData
	{
#pragma region Using
		using UserData = std::pair<ColliderComponent*, ECS::EntityID>;
#pragma endregion

		FF_AABB		m_aabb;
		UserData*	m_userData;

		QuadTreeNodeData()
		{}

		QuadTreeNodeData(FF_AABB aabb, UserData* userData) :
			m_aabb(aabb),
			m_userData(userData)
		{}

		~QuadTreeNodeData()
		{
			m_userData = nullptr;
		}

		bool operator==	(const QuadTreeNodeData& other)
		{
			return m_aabb == other.m_aabb;
		}

		bool operator!=	(const QuadTreeNodeData& other)
		{
			return m_aabb != other.m_aabb;
		}
	};


	class QuadTreeNode : public Service::FFE_SERV_USER_MemoryManager
	{
#pragma region Using
		using BoundingBox = FF_AABB;
		using Allocator = FlatFormer::Memory::Allocator::IAllocator;
		using nodeDataList = std::vector<QuadTreeNodeData>;
		using UserData = std::pair<ColliderComponent*, ECS::EntityID >;
#pragma endregion

	protected:
	
		QuadTreeNode *	m_leftTop	= nullptr;
		QuadTreeNode *	m_leftBot	= nullptr;
		QuadTreeNode *	m_rightTop	= nullptr;
		QuadTreeNode *	m_rightBot	= nullptr;
		QuadTreeNode *	m_parent	= nullptr;

		Allocator *		m_allocator = nullptr;

		nodeDataList *	m_points	= nullptr;	// list of aabb included in the node

		size_t			m_capacity		= 0;	// node capacity
		size_t			m_count			= 0;	// number of added elements
		size_t			m_deepLevel		= 0;	// maximum division depth

		bool			m_isAdded	= false;	// flag indicating whether node points have been added
		bool			m_inAllocatorMemory;	// flag indicating the ownership of the memory by the allocator

		BoundingBox		m_boundary;				// node bounding box

	public:

		QuadTreeNode(BoundingBox boundary, int capacity , size_t deepLevel, QuadTreeNode * parent, Allocator* allocator);
		~QuadTreeNode();


		bool Insert(const FF_AABB& aabb, UserData* userData);


		bool Remove(const FF_AABB& aabb);


		// Recursively removes all child nodes
		void RemoveNodes();


		// Populaes the aabb array with which to check
		void FillUserDataList(const FF_AABB& bound, std::vector<UserData*>& outList);


		// Recursively removes nodes and then clears the list
		void Clear();


		// Performs splitting of the current node
		void Subdevide();


		// Returns the number of elements in a node
		inline size_t GetCount()
		{
			return m_count;
		}

		// populates list of aabb
		void FillDebug(std::vector<FF_AABB>& outList);
	};

}}