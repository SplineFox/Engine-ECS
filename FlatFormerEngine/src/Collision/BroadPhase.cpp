#include "Collision/BroadPhase.h"

namespace FlatFormer {namespace Collision {

	BroadPhase::BroadPhase()
	{
		m_grid = new SeparationGrid(FFE_SEPARATION_CELL_WIDTH_DEFAULT, FFE_SEPARATION_CELL_HEIGHT_DEFAULT);
		m_colliders.reserve(200);
		m_oldAABBs.reserve(200);
	}

	BroadPhase::~BroadPhase()
	{
		delete m_grid;
		m_grid = nullptr;

		m_colliders.clear();
		m_oldAABBs.clear();
		m_broadColliders.clear();
	}


	void BroadPhase::AddToPhase(ColliderAndId* userData)
	{
		m_colliders.emplace_back(userData);
		m_oldAABBs.emplace_back(userData->first->m_collider->GetAABB());

		m_grid->AddToGrid(userData->first->m_collider->GetAABB(), userData);
	}


	void BroadPhase::DeleteFromPhase(ColliderAndId* userData)
	{
		m_colliders.erase(std::remove(m_colliders.begin(), m_colliders.end(), userData), m_colliders.end());
		m_oldAABBs.erase(std::remove(m_oldAABBs.begin(), m_oldAABBs.end(), userData->first->m_collider->GetAABB()), m_oldAABBs.end());

		m_grid->RemoveFromGrid(userData->first->m_collider->GetAABB());
	}


	void BroadPhase::Update()
	{
		this->UdateGrid();

		m_broadColliders.clear();

		std::vector<ColliderAndId*> outList;

		FF_AABB aabbCache;

		for (size_t i = 0; i < m_oldAABBs.size(); i++)
		{
			m_grid->FillUserDataList(m_oldAABBs[i], outList);

			for (size_t j = 0; j < outList.size(); j++)
			{
				aabbCache = outList[j]->first->m_collider->GetAABB();

				/// it is necessary to choose where the elements in the pair will be located so that there are no repetitions in the set
				if (m_oldAABBs[i] != aabbCache && m_oldAABBs[i].isOverLap(aabbCache))
				{
					if (m_oldAABBs[i].minCorner.x > aabbCache.minCorner.x)
					{
						m_broadColliders.insert(std::make_pair(m_colliders[i], outList[j]));
						continue;
					}
					else if (m_oldAABBs[i].minCorner.x < aabbCache.minCorner.x)
					{
						m_broadColliders.insert(std::make_pair(outList[j], m_colliders[i]));
						continue;
					}

					if (m_oldAABBs[i].minCorner.y > aabbCache.minCorner.y)
					{
						m_broadColliders.insert(std::make_pair(m_colliders[i], outList[j]));
						continue;
					}
					else if (m_oldAABBs[i].minCorner.y < aabbCache.minCorner.y)
					{
						m_broadColliders.insert(std::make_pair(outList[j], m_colliders[i]));
						continue;
					}

					m_broadColliders.insert(std::make_pair(outList[j], m_colliders[i]));
				}
			}

			outList.clear();
		}
	}


	void BroadPhase::UdateGrid()
	{
		size_t count = m_colliders.size();

		for (size_t i = 0; i < count; i++)
		{
			if (m_colliders[i]->first->m_collider->GetAABB().minCorner != m_oldAABBs[i].minCorner)
			{
				m_grid->RemoveFromGrid(m_oldAABBs[i]);

				m_oldAABBs[i] = m_colliders[i]->first->m_collider->GetAABB();

				m_grid->AddToGrid(m_oldAABBs[i], m_colliders[i]);
			}
		}

		m_grid->Update();
	}

}}