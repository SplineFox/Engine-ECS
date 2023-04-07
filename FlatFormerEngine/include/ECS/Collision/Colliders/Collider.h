#pragma once

#include "ECS/Component.h"


#include "Engine/API.h"
#include "Engine/Platform.h"


#include "Math/vec2.h"
#include "Math/Transform2D.h"


#include "Collision/FF_AABB.h"


namespace FlatFormer {
	namespace Collision {

		enum colliderType
		{
			circle,
			polygon
		};


		class Collider
		{

		protected:
			vec2			m_offset;		// offset relative to transform
			vec2			m_center;		// geometric center of the shape
			float			m_radius;		// shape rotation radius
			FF_AABB			m_aabb;			// AABB
			colliderType    m_type;			// type
			bool			m_isTrigger;	// whether the collider is a trigger or not

		public:

			Collider()
			{
				m_offset = vec2();
				m_center = vec2();
				m_radius = 0;
				m_isTrigger = false;
			}


			virtual ~Collider()
			{}


			inline vec2 GetCentroid() const
			{
				return m_center;
			}


			inline float GetRadius() const
			{
				return m_radius;
			}


			inline const colliderType GetType() const
			{
				return m_type;
			}


			inline const FF_AABB& GetAABB() const
			{
				return m_aabb;
			}


			inline vec2 GetOffset() const
			{
				return m_offset;
			}


			inline void SetOffset(vec2 offset)
			{
				m_offset = offset;
			}


			inline bool IsTrigger() const
			{
				return m_isTrigger;
			}


			inline void SetTrigger(bool isTrigger)
			{
				m_isTrigger = isTrigger;
			}


			inline void ComputeAABB()
			{
				m_aabb.minCorner = m_center - vec2(m_radius, m_radius);
				m_aabb.maxCorner = m_center + vec2(m_radius, m_radius);
			}


			virtual void UpdateWorldPositions(FlatFormer::Transform2D * transform) = 0;
		};
	}
}
