#pragma once
#include "ECS/Collision/Colliders/Collider.h"

namespace FlatFormer { namespace Collision {

	// Represents an edge of the polygon
	struct edge
	{
		vec2 v1;
		vec2 v2;
	};


	class PolygonCollider : public Collider
	{

#pragma region Using
		using vec2Vector = std::vector<vec2>;

#pragma endregion

	private:
		vec2Vector* m_localVertices;
		vec2Vector* m_worldVertices;
		vec2		m_localCentr;

		FFE_DECLARE_LOGGER

		void ComputeRadius();

	public:

		PolygonCollider();
		PolygonCollider(const PolygonCollider & collider);
		~PolygonCollider();


		void AddVertex(vec2 point);


		void DeleteVertex(uint8 index);


		void SetVertices(const vec2Vector& vertices);


		inline vec2 GetLocalVertex(uint8 index) const
		{
			return (*m_localVertices)[index];
		};


		inline const vec2Vector* GetLocalVertices() const
		{
			return m_localVertices;
		};


		inline vec2 GetWorldVertex(uint8 index) const
		{
			return (*m_worldVertices)[index];
		};


		inline const vec2Vector* GetWorldVertices() const
		{
			return m_worldVertices;
		};


		inline uint8 GetVerticesCount() const { return m_localVertices->size(); };


		void UpdateWorldPositions(Transform2D * transform);


		// returns nearest edges to intersect
		static edge GetFaceEdge(const vec2Vector& vertices, vec2 normal)
		{
			size_t count = vertices.size();
			size_t index = 0;

			float projection;
			float max = -FFE_MAX_FLOAT;

			for (size_t i = 0; i < count; i++)
			{
				projection = vec2::Dot(normal, vertices.at(i));

				if (projection > max) {
					max = projection;
					index = i;
				}
			}

			vec2 v = vertices[index];

			vec2 v1(0, 0);
			vec2 v0(0, 0);

			if (index == count - 1)
			{
				v1 = vertices[0];
				v0 = vertices[index - 1];
			}
			else if (index == 0)
			{
				v1 = vertices[index + 1];
				v0 = vertices[count - 1];
			}
			else
			{
				v1 = vertices[index + 1];
				v0 = vertices[index - 1];
			}


			vec2 l = v - v1;
			vec2 r = v - v0;

			l = vec2::Normalize(l);
			r = vec2::Normalize(r);

			edge resutEdge;

			if (vec2::Dot(normal, r) <= vec2::Dot(normal, l))
			{
				resutEdge.v1 = v0;
				resutEdge.v2 = v;
			}
			else
			{
				resutEdge.v1 = v;
				resutEdge.v2 = v1;
			}

			return resutEdge;
		};


		static vec2Vector * ComputeNormals(const vec2Vector& vertices)
		{
			size_t count = vertices.size();

			if (count < 2)
			{
				return nullptr;
			}

			vec2Vector* normals = new vec2Vector(count);

			for (size_t i = 0; i < count - 1; i++)
			{
				normals->at(i) = vec2::GetLeftNormal(vertices[i + 1] - vertices[i]);
			}

			(*normals)[count - 1] = vec2::GetLeftNormal(vertices[0] - vertices[count - 1]);
			
			return normals;
		};


		// Calculates the geometric center of the polygon
		static vec2 ComputeCentroid(const vec2Vector *const vertices)
		{
			size_t count = vertices->size();

			if (count == 0)
			{
				return vec2::Zero();
			}

			vec2 centr = (*vertices)[0];

			for (size_t i = 1; i < count; i++)
			{
				centr = centr + ((*vertices)[i] - centr) / (i + 1);
			}

			return centr;
		}


		// Returns the interval of the figure's projection onto the axis
		static vec2 ProjectOnNormal(const vec2Vector& vertices, vec2 normal)
		{
			if (vertices.size() <= 0)
			{
				return vec2::Zero();
			}


			float	cacheProjection = vec2::Dot(vertices.front(), normal);
			vec2	projection = vec2(cacheProjection, cacheProjection);

			vec2Vector::const_iterator iter = vertices.begin();
			iter++;

			while (iter != vertices.end())
			{
				cacheProjection = vec2::Dot(*iter, normal);

				if (projection.x > cacheProjection)
					projection.x = cacheProjection;

				if (projection.y < cacheProjection)
					projection.y = cacheProjection;

				iter++;
			}

			return projection;
		}

		float ComputeMass(float density)
		{
			if (m_localVertices->size() < 3)
			{
				return 0;
			}

			float area = 0;
			uint8 count = m_localVertices->size() - 1;

			for (size_t i = 0; i < count; i++)
			{
				area += 0.5 * FFE_FABSF(vec2::Cross((*m_localVertices)[i], (*m_localVertices)[i + 1]));
			}

			area += 0.5 * FFE_FABSF(vec2::Cross((*m_localVertices)[count], (*m_localVertices)[0]));

			return density * area;
		}

		float ComputeInertia(float density)
		{
			if (m_localVertices->size() < 3)
			{
				return 0;
			}

			float inertia = 0;
			uint8 count = m_localVertices->size() - 1;

			float mass_tri;
			float inertia_tri;

			for (size_t i = 0; i < count; i++)
			{
				mass_tri = density * 0.5 * FFE_FABSF(vec2::Cross((*m_localVertices)[i], (*m_localVertices)[i + 1]));
				inertia_tri = mass_tri * (((*m_localVertices)[i].Length() + (*m_localVertices)[i + 1].Length() + vec2::Dot((*m_localVertices)[i], (*m_localVertices)[i + 1]))) / 6;
				inertia += inertia_tri;
			}

			mass_tri = density * 0.5 * FFE_FABSF(vec2::Cross((*m_localVertices)[count], (*m_localVertices)[0]));
			inertia_tri = mass_tri * (((*m_localVertices)[count].Length() + (*m_localVertices)[0].Length() + vec2::Dot((*m_localVertices)[count], (*m_localVertices)[0]))) / 6;
			inertia += inertia_tri;

			return inertia;
		}

	};
}}