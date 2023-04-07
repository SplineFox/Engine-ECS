#include"Collision/QuadTree/QuadTree.h"

namespace FlatFormer { namespace Collision {

	QuadTree::QuadTree(BoundingBox boundary, int capacity, PoolAllocator*	nodeAllocator)
	{
		m_nodeAllocator = nodeAllocator;
		m_rootNode = new QuadTreeNode(boundary, capacity, 0, nullptr, m_nodeAllocator);
	}


	QuadTree::~QuadTree()
	{
		delete m_rootNode;
		m_nodeAllocator = nullptr;
	}


	void QuadTree::Insert(const FF_AABB& aabb, UserData* userData)
	{
		m_rootNode->Insert(aabb, userData);
	}


	void QuadTree::Remove(const FF_AABB& aabb)
	{
		m_rootNode->Remove(aabb);
	}


	void QuadTree::FillUserDataList(const FF_AABB& bound, std::vector<UserData*>& outList)
	{
		m_rootNode->FillUserDataList(bound, outList);
	}


	void QuadTree::Clear()
	{
		m_rootNode->Clear();
	}


	void QuadTree::FillDebug(std::vector<FF_AABB>& outList)
	{
		m_rootNode->FillDebug(outList);
	}


}}