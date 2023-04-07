#include "ECS/Collision/Colliders/PolygonCollider.h"
#include "Math/FFMath.h"

namespace FlatFormer { namespace Collision {
	
	PolygonCollider::PolygonCollider()		
	{
		m_localVertices = new vec2Vector();
		m_worldVertices = new vec2Vector();

		m_localVertices->reserve(FFE_MAX_VERTICES_COUNT);
		m_worldVertices->reserve(FFE_MAX_VERTICES_COUNT);

		m_localCentr = vec2();

		m_type = colliderType::polygon;

		FFE_DEFINE_LOGGER("PolygonCollider")
		FFE_LOG_DEBUG("Initialized")
	}


	PolygonCollider::PolygonCollider(const PolygonCollider & collider):
		m_localVertices(collider.m_localVertices),
		m_worldVertices(collider.m_worldVertices)
	{
		this->m_offset	= collider.m_offset;
		this->m_center	= collider.m_center;
		this->m_radius	= collider.m_radius;
		this->m_aabb	= collider.m_aabb;

		m_localCentr	= collider.m_localCentr;

		m_type = colliderType::polygon;

		FFE_DEFINE_LOGGER("PolygonCollider")
		FFE_LOG_DEBUG("Initialized")
	}

	PolygonCollider::~PolygonCollider()
	{
		delete m_localVertices;
		delete m_worldVertices;
	}


	void PolygonCollider::AddVertex(vec2 point)
	{
		if (m_localVertices->size() > FFE_MAX_VERTICES_COUNT)
		{
			FFE_LOG_DEBUG("Polygon vertices is full")

			return;
		}

		m_localVertices->push_back(point);
		m_worldVertices->push_back(point);

		m_localCentr = PolygonCollider::ComputeCentroid(m_localVertices);

		vec2 distance = m_localCentr - m_localVertices->back();
		m_center = m_localCentr;
		this->ComputeRadius();
		this->ComputeAABB();
	}


	void PolygonCollider::DeleteVertex(uint8 index)
	{
		if (index >= m_localVertices->size() || index < 0)
		{
			FFE_LOG_DEBUG("Invalid vertex delete index")
			return;
		}

		m_localVertices->erase(std::remove(m_localVertices->begin(), m_localVertices->end(), m_localVertices->at(index)), m_localVertices->end());
		m_worldVertices->erase(std::remove(m_worldVertices->begin(), m_worldVertices->end(), m_worldVertices->at(index)), m_worldVertices->end());

		m_localCentr = PolygonCollider::ComputeCentroid(m_localVertices);
		m_center = m_localCentr;

		this->ComputeRadius();
		this->ComputeAABB();
	}


	void PolygonCollider::SetVertices(const vec2Vector& vertices)
	{
		if (vertices.size() > FFE_MAX_VERTICES_COUNT)
		{
			FFE_LOG_DEBUG("Invalid vertex vector size")
			return;
		}

		m_localVertices->clear();
		m_localVertices->insert(m_localVertices->end(), vertices.begin(), vertices.end());	

		m_worldVertices->clear();
		m_worldVertices->insert(m_worldVertices->end(), vertices.begin(), vertices.end());

		m_localCentr = PolygonCollider::ComputeCentroid(m_localVertices);
		m_center = m_localCentr;

		this->ComputeRadius();
		this->ComputeAABB();
	}


	void PolygonCollider::UpdateWorldPositions(Transform2D * transform)
	{	
		for (size_t i = 0; i < m_localVertices->size(); i++)
		{
			(*m_worldVertices)[i]= transform->ConvertToWorld((*m_localVertices)[i] + m_offset);
		}

		m_center = transform->ConvertToWorld(m_localCentr + m_offset);

		this->ComputeAABB();
	}


	void PolygonCollider::ComputeRadius()
	{
		vec2 fromTo;

		size_t index	= 0;
		size_t count	= m_localVertices->size();

		float max		= 0;

		for (size_t i = 0; i < count; i++)
		{
			fromTo = m_localVertices->at(i) - m_localCentr;

			if (max*max < fromTo.x*fromTo.x + fromTo.y*fromTo.y)
			{
				max = fromTo.Length();
			}
		}

		this->m_radius = max;
	}

}}