#include "ECS/Collision/Colliders/CircleCollider.h"
#include "Math/FFMath.h"

namespace FlatFormer { namespace Collision {

	CircleCollider::CircleCollider()
	{
		FFE_DEFINE_LOGGER("CircleCollider")
		FFE_LOG_DEBUG("Initialized")

		m_worldPosition = m_offset;

		this->m_radius	= 1;

		this->ComputeAABB();

		m_type = colliderType::circle;	
	}

	CircleCollider::CircleCollider(float radius) 
	{
		FFE_DEFINE_LOGGER("CircleCollider")
		FFE_LOG_DEBUG("Initialized")

		m_worldPosition = m_offset;

		this->SetRadius(radius);
		this->ComputeAABB();

		m_type = colliderType::circle;
	}

	CircleCollider::CircleCollider(const CircleCollider & collider)
	{
		FFE_DEFINE_LOGGER("CircleCollider")
		FFE_LOG_DEBUG("Initialized")

		m_worldPosition = collider.m_worldPosition;
		this->m_offset	= collider.m_offset;
		this->m_center	= collider.m_center;
		this->m_radius	= collider.m_radius;
		this->m_aabb	= collider.m_aabb;

		m_type = colliderType::circle;
	}

	void CircleCollider::UpdateWorldPositions(Transform2D * transform)
	{
		m_worldPosition = transform->ConvertToWorld(this->m_offset);
		m_center = m_worldPosition;
		this->ComputeAABB();
	}

}}