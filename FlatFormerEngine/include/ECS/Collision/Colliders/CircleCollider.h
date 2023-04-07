#pragma once

#include "ECS/Collision/Colliders/Collider.h"

namespace FlatFormer { namespace Collision {

	class CircleCollider :public Collider
	{
	private:
		vec2	m_worldPosition;

		FFE_DECLARE_LOGGER

	public:

		CircleCollider();
		CircleCollider(float radius);
		CircleCollider(const CircleCollider & collider);
		~CircleCollider() {}


		inline void SetRadius(float radius)
		{
			if (radius < 0)
			{
				FFE_LOG_DEBUG("Radius < 0")
				return;
			}

			this->m_radius = radius;

			this->ComputeAABB();
		}


		inline vec2 GetWorldPosition() const
		{
			return m_worldPosition;
		}


		void UpdateWorldPositions(FlatFormer::Transform2D * transform);


		static vec2	ProjectOnNormal(const CircleCollider* circle, vec2 n)
		{
			float	cacheProjection = vec2::Dot(circle->GetWorldPosition(), n);
			vec2	projection = vec2(cacheProjection - circle->GetRadius(), cacheProjection + circle->GetRadius());

			return projection;
		}

		float ComputeMass(float density)
		{
			return density * FFE_PI * m_radius* m_radius;
		}

		float ComputeInertia(float mass)
		{
			return 0.5f * mass * m_radius * m_radius;
		}
	};

}}