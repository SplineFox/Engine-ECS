#include "Collision/SeparationGrid.h"
#include "Math/FFMath.h"

namespace FlatFormer { namespace Collision {

	SeparationGrid::SeparationGrid(size_t cellSizeX, size_t cellSizeY) :
		m_cellSizeX(cellSizeX),
		m_cellSizeY(cellSizeY)
	{
		m_treeAllocator = new PoolAllocator
		(
			FFE_MAX_TREE_COUNT * sizeof(QuadTree),
			this->GetMemoryManagerService().Allocate(FFE_MAX_TREE_COUNT * sizeof(QuadTree), "SeparationGrid"),
			sizeof(QuadTree)
		);

		m_nodeAllocator = new PoolAllocator
		(
			FFE_MAX_TREE_MEMORY_CAPACITY,
			this->GetMemoryManagerService().Allocate(FFE_MAX_TREE_MEMORY_CAPACITY, "QuadTrees"),
			sizeof(QuadTreeNode)
		);
	}

	SeparationGrid::~SeparationGrid()
	{
		for (auto item: m_cellsMap)
		{
			item.second->~QuadTree();
			m_treeAllocator->Deallocate(item.second);
			item.second = nullptr;
		}

		m_cellsMap.clear();

		this->GetMemoryManagerService().Deallocate((void*)m_treeAllocator->GetMemoryAddress0());
		m_treeAllocator->~PoolAllocator();
		m_treeAllocator = nullptr;

		this->GetMemoryManagerService().Deallocate((void*)m_nodeAllocator->GetMemoryAddress0());
		m_nodeAllocator->~PoolAllocator();
		m_nodeAllocator = nullptr;
	}

	void SeparationGrid::AddToGrid(const FF_AABB& aabb, UserData* userData)
	{
		vec2 indexMin;
		vec2 indexMax;

		/// find indexes of cells to insert
		indexMin.x = (int)(aabb.minCorner.x / m_cellSizeX);
		indexMin.y = (int)(aabb.minCorner.y / m_cellSizeY);

		if (FF_Signf(aabb.minCorner.x) < 0)
		{
			indexMin.x += -1;
		}

		if (FF_Signf(aabb.minCorner.y) < 0)
		{
			indexMin.y += -1;
		}

		indexMax.x = (int)(aabb.maxCorner.x / m_cellSizeX);
		indexMax.y = (int)(aabb.maxCorner.y / m_cellSizeY);

		if (FF_Signf(aabb.maxCorner.x) < 0)
		{
			indexMax.x += -1;
		}

		if (FF_Signf(aabb.maxCorner.y) < 0)
		{
			indexMax.y += -1;
		}

		size_t iyCount = indexMax.y - indexMin.y + 1;
		size_t jxCount = indexMax.x - indexMin.x + 1;

		vec2 minCorner;
		vec2 maxCormer;

		vec2 key;

		QuadTree* tree;
		GridMap::iterator iter;

		for (size_t i = 0; i < iyCount; i++)
		{
			for (size_t j = 0; j < jxCount; j++)
			{
				minCorner = vec2(indexMin.x + j, indexMin.y + i);
				maxCormer = minCorner;

				minCorner.x = minCorner.x * m_cellSizeX;
				minCorner.y = minCorner.y * m_cellSizeY;

				maxCormer.x = minCorner.x + m_cellSizeX;
				maxCormer.y = minCorner.y + m_cellSizeY;

				key.x = indexMin.x + j;
				key.y = indexMin.y + i;

				iter = m_cellsMap.find(key);

				if (iter != m_cellsMap.end())
				{
					iter->second->Insert(aabb, userData);
					continue;
				}

				void * treePtr = m_treeAllocator->Allocate(sizeof(QuadTree));
				tree = new(treePtr)QuadTree(BoundingBox(minCorner, maxCormer), FFE_MAX_TREE_NODE_CAPACITY, m_nodeAllocator);

				tree->Insert(aabb, userData);
				
				m_cellsMap.emplace(key, tree);
			}
		}

		tree = nullptr;
	}

	void SeparationGrid::RemoveFromGrid(const FF_AABB& aabb)
	{
		vec2 indexMin;
		vec2 indexMax;

		/// find indexes of cells to delete
		indexMin.x = (int)(aabb.minCorner.x / m_cellSizeX);
		indexMin.y = (int)(aabb.minCorner.y / m_cellSizeY);

		if (FF_Signf(aabb.minCorner.x) < 0)
		{
			indexMin.x += -1;
		}

		if (FF_Signf(aabb.minCorner.y) < 0)
		{
			indexMin.y += -1;
		}

		indexMax.x = (int)(aabb.maxCorner.x / m_cellSizeX);
		indexMax.y = (int)(aabb.maxCorner.y / m_cellSizeY);

		if (FF_Signf(aabb.maxCorner.x) < 0)
		{
			indexMax.x += -1;
		}

		if (FF_Signf(aabb.maxCorner.y) < 0)
		{
			indexMax.y += -1;
		}

		size_t iyCount = indexMax.y - indexMin.y + 1;
		size_t jxCount = indexMax.x - indexMin.x + 1;

		vec2 key;
		GridMap::iterator iter;

		for (size_t i = 0; i < iyCount; i++)
		{
			for (size_t j = 0; j < jxCount; j++)
			{
				key.x = indexMin.x + j;
				key.y = indexMin.y + i;

				iter = m_cellsMap.find(key);
				iter->second-> Remove(aabb);		
			}
		}
	}

	
	void SeparationGrid::FillUserDataList(const FF_AABB& aabb, std::vector<UserData*>& outList)
	{
		vec2 indexMin;
		vec2 indexMax;

		/// find cell indices to get list
		indexMin.x = (int)(aabb.minCorner.x / m_cellSizeX);
		indexMin.y = (int)(aabb.minCorner.y / m_cellSizeY);

		if (FF_Signf(aabb.minCorner.x) < 0)
		{
			indexMin.x += -1;
		}

		if (FF_Signf(aabb.minCorner.y) < 0)
		{
			indexMin.y += -1;
		}

		indexMax.x = (int)(aabb.maxCorner.x / m_cellSizeX);
		indexMax.y = (int)(aabb.maxCorner.y / m_cellSizeY);

		if (FF_Signf(aabb.maxCorner.x) < 0)
		{
			indexMax.x += -1;
		}

		if (FF_Signf(aabb.maxCorner.y) < 0)
		{
			indexMax.y += -1;
		}

		size_t iyCount = indexMax.y - indexMin.y + 1;
		size_t jxCount = indexMax.x - indexMin.x + 1;

		vec2 key;
		GridMap::iterator iter;

		for (size_t i = 0; i < iyCount; i++)
		{
			for (size_t j = 0; j < jxCount; j++)
			{
				key.x = indexMin.x + j;
				key.y = indexMin.y + i;
				iter = m_cellsMap.find(key);
				iter->second->FillUserDataList(aabb, outList);
			}
		}
	}
	

	void SeparationGrid::Update()
	{
		auto iter = m_cellsMap.begin();
		while (iter!= m_cellsMap.end())
		{
			if (iter->second->IsEmpty())
			{
				iter->second->~QuadTree();
				m_treeAllocator->Deallocate(iter->second);
				iter = m_cellsMap.erase(iter);
				continue;
			}
			
			iter++;
		}
	}


	void SeparationGrid::Clear()
	{
		for (auto item : m_cellsMap)
		{
			item.second->~QuadTree();
			m_treeAllocator->Deallocate(item.second);
			item.second = nullptr;
		}

		m_cellsMap.clear();
	}

}}