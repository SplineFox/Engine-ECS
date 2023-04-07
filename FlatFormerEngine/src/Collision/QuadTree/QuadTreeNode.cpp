#include"Collision/QuadTree/QuadTreeNode.h"

namespace FlatFormer { namespace Collision {

	QuadTreeNode::QuadTreeNode(BoundingBox boundary, int capacity, size_t deepLevel, QuadTreeNode * parent, Allocator* allocator):
		m_parent(parent),
		m_allocator(allocator),
		m_capacity(capacity),
		m_deepLevel(deepLevel),
		m_boundary(boundary)
	{	
		m_points = new nodeDataList();
		m_points->reserve(capacity);
	}


	QuadTreeNode::~QuadTreeNode()
	{
		this->RemoveNodes();

		delete m_points;
		m_points = nullptr;

		m_parent	= nullptr;
		m_allocator = nullptr;
	}


	bool QuadTreeNode::Insert(const FF_AABB& aabb, UserData* userData)
	{
		if (m_parent == nullptr)
		{
			// if aabb intersects the current node, then insert
			if (!m_boundary.isOverLap(aabb))
			{
				return false;
			}
		}
		else
		{
			// if aabb is contained in the current node, then insert
			if (!m_boundary.IsContain(aabb))
			{
				return false;
			}
		}

		/// if the number of elements in the current node allows insertion into it, then insert
		if (m_count < m_capacity)
		{
			m_points->push_back(QuadTreeNodeData(aabb, userData));
			m_count++;

			return true;
		} 


		/// otherwise, we look to see if the node is divided by 4, if not, then we divide and distribute the elements among the child nodes,
		/// 
		/// if the element cannot be placed in any child node, then leave it in the current
		if (m_leftTop == nullptr && m_deepLevel < FFE_MAX_TREE_DEEP)
		{
			this->Subdevide();

			nodeDataList::iterator iter = m_points->begin();

			while (iter != m_points->end())
			{
				if (m_leftTop->Insert(iter->m_aabb, iter->m_userData))
				{
					iter = m_points->erase(iter);
					continue;
				};

				if (m_leftBot->Insert(iter->m_aabb, iter->m_userData))
				{
					iter = m_points->erase(iter);
					continue;
				};

				if (m_rightTop->Insert(iter->m_aabb, iter->m_userData))
				{
					iter = m_points->erase(iter);
					continue;
				};

				if (m_rightBot->Insert(iter->m_aabb, iter->m_userData))
				{
					iter = m_points->erase(iter);
					continue;
				};

				iter++;
			}
		}

		m_count++;

		if (m_leftTop != nullptr)
		{
			if (m_leftTop->Insert(aabb, userData)) return true;
			if (m_leftBot->Insert(aabb, userData)) return true;
			if (m_rightTop->Insert(aabb, userData)) return true;
			if (m_rightBot->Insert(aabb, userData)) return true;
		}

		if (m_points->size() >= m_points->capacity())
		{
			m_points->reserve(m_capacity);
		}

		m_points->push_back(QuadTreeNodeData(aabb, userData));

		return true;
	}


	bool QuadTreeNode::Remove(const FF_AABB& aabb)
	{
		/// check needed for boundary aabb
		if (m_parent == nullptr)
		{
			/// if aabb intersects the current node, then delete
			if (!m_boundary.isOverLap(aabb))
			{
				return false;
			}
		}
		else
		{
			/// if aabb is contained in the current node, then delete
			if (!m_boundary.IsContain(aabb))
			{
				return false;
			}
		}

		UserData* cache = 0;

		if (m_leftTop == nullptr)
		{
			m_points->erase(std::remove(m_points->begin(), m_points->end(), QuadTreeNodeData(aabb, cache)), m_points->end());
			m_count--;
			cache = nullptr;
			return true;
		}


		/// if the number of elements in the node allows you to merge child nodes,
		/// then we do this so that there are no empty branches in the tree
		if (m_count <= m_capacity)
		{
			m_points->insert(m_points->begin(), m_leftTop->m_points->begin(), m_leftTop->m_points->end());
			m_points->insert(m_points->begin(), m_leftBot->m_points->begin(), m_leftBot->m_points->end());
			m_points->insert(m_points->begin(), m_rightTop->m_points->begin(), m_rightTop->m_points->end());
			m_points->insert(m_points->begin(), m_rightBot->m_points->begin(), m_rightBot->m_points->end());

			this->RemoveNodes();

			m_points->erase(std::remove(m_points->begin(), m_points->end(), QuadTreeNodeData(aabb, cache)), m_points->end());
			m_count--;

			cache = nullptr;

			return true;
		}

		if (m_leftTop->Remove(aabb)) { m_count--; return true; }
		if (m_leftBot->Remove(aabb)) { m_count--; return true; }
		if (m_rightTop->Remove(aabb)) { m_count--; return true; }
		if (m_rightBot->Remove(aabb)) { m_count--; return true; }

		m_points->erase(std::remove(m_points->begin(), m_points->end(), QuadTreeNodeData(aabb, cache)), m_points->end());

		m_count--;

		cache = nullptr;

		return true;
	}


	void QuadTreeNode::RemoveNodes()
	{
		if (m_leftTop == nullptr)
		{
			return;
		}

		if (m_inAllocatorMemory)
		{
			m_leftTop->~QuadTreeNode();
			m_leftBot->~QuadTreeNode();
			m_rightTop->~QuadTreeNode();
			m_rightBot->~QuadTreeNode();

			m_allocator->Deallocate(m_leftTop);
			m_allocator->Deallocate(m_leftBot);
			m_allocator->Deallocate(m_rightTop);
			m_allocator->Deallocate(m_rightBot);

			m_leftTop = nullptr;
			m_leftBot = nullptr;
			m_rightTop = nullptr;
			m_rightBot = nullptr;
			return;

		}

		delete m_leftTop;
		delete m_leftBot;
		delete m_rightTop;
		delete m_rightBot;

		m_leftTop = nullptr;
		m_leftBot = nullptr;
		m_rightTop = nullptr;
		m_rightBot = nullptr;
	}


	void QuadTreeNode::FillUserDataList(const FF_AABB& aabb, std::vector<UserData*>& outList)
	{
		m_isAdded = false;

		/// check needed for boundary aabb
		if (m_parent == nullptr)
		{
			/// if aabb does not cross the current node, then do nothing
			if (!m_boundary.isOverLap(aabb))
			{
				return;
			}

			for (size_t i = 0; i < m_points->size(); i++)
			{
				outList.emplace_back((*m_points)[i].m_userData);
			}

			m_isAdded = true;
		}
		else
		{
			/// if aabb is contained in the current node, then fill
			if (!m_boundary.IsContain(aabb))
			{
				if (!m_parent->m_isAdded)
				{
					for (size_t i = 0; i < m_parent->m_points->size(); i++)
					{
						outList.emplace_back((*m_parent->m_points)[i].m_userData);
					}

					m_parent->m_isAdded = true;
				}

				return;
			}
		}

		if (m_leftTop == nullptr)
		{
			if (!m_isAdded)
			{
				for (size_t i = 0; i < m_points->size(); i++)
				{
					outList.emplace_back((*m_points)[i].m_userData);
				}

				m_isAdded = true;
			}

			if (m_parent != nullptr && !m_parent->m_isAdded)
			{
				for (size_t i = 0; i < m_parent->m_points->size(); i++)
				{
					outList.emplace_back((*m_parent->m_points)[i].m_userData);
				}

				m_parent->m_isAdded = true;
			}

			return;
		}

		m_leftTop->FillUserDataList(aabb, outList);
		m_leftBot->FillUserDataList(aabb, outList);
		m_rightTop->FillUserDataList(aabb, outList);
		m_rightBot->FillUserDataList(aabb, outList);
	}


	void QuadTreeNode::Clear()
	{
		this->RemoveNodes();

		m_points->clear();
		m_count = 0;
	}


	void QuadTreeNode::Subdevide()
	{
		BoundingBox box = m_boundary;

		double xMid = (box.minCorner.x + box.maxCorner.x) / 2.0;
		double yMid = (box.minCorner.y + box.maxCorner.y) / 2.0;

		/// calculate bounding boxes of child nodes
		BoundingBox leftTopBox = BoundingBox(vec2(box.minCorner.x, yMid), vec2(xMid, box.maxCorner.y));
		BoundingBox leftBotBox = BoundingBox(box.minCorner, vec2(xMid, yMid));
		BoundingBox rightTopBox = BoundingBox(vec2(xMid, yMid), box.maxCorner);
		BoundingBox rightBotBox = BoundingBox(vec2(xMid, box.minCorner.y), vec2(box.maxCorner.x, yMid));

		if (!m_allocator->IsFull())
		{
			void* m_leftTopPtr = m_allocator->Allocate(sizeof(QuadTreeNode));
			void* m_leftBotPtr = m_allocator->Allocate(sizeof(QuadTreeNode));
			void* m_rightTopPtr = m_allocator->Allocate(sizeof(QuadTreeNode));
			void* m_rightBotPtr = m_allocator->Allocate(sizeof(QuadTreeNode));

			m_leftTop = new (m_leftTopPtr)QuadTreeNode(leftTopBox, m_capacity, m_deepLevel + 1, this, m_allocator);
			m_leftBot = new (m_leftBotPtr)QuadTreeNode(leftBotBox, m_capacity, m_deepLevel + 1, this, m_allocator);
			m_rightTop = new (m_rightTopPtr)QuadTreeNode(rightTopBox, m_capacity, m_deepLevel + 1, this, m_allocator);
			m_rightBot = new (m_rightBotPtr)QuadTreeNode(rightBotBox, m_capacity, m_deepLevel + 1, this, m_allocator);

			m_inAllocatorMemory = true;

			return;
		}


		m_leftTop = new QuadTreeNode(leftTopBox, m_capacity, m_deepLevel + 1, this, m_allocator);
		m_leftBot = new QuadTreeNode(leftBotBox, m_capacity, m_deepLevel + 1, this, m_allocator);
		m_rightTop = new QuadTreeNode(rightTopBox, m_capacity, m_deepLevel + 1, this, m_allocator);
		m_rightBot = new QuadTreeNode(rightBotBox, m_capacity, m_deepLevel + 1, this, m_allocator);

		m_inAllocatorMemory = false;
	}

	// populates list of aabb
	void QuadTreeNode::FillDebug(std::vector<FF_AABB>& outList)
	{
		if (m_leftTop == nullptr)
		{
			outList.push_back(this->m_boundary);
			return;
		}

		m_leftTop->FillDebug(outList);
		m_leftBot->FillDebug(outList);
		m_rightTop->FillDebug(outList);
		m_rightBot->FillDebug(outList);

	}

}}