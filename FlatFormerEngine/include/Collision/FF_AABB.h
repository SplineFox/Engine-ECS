#pragma once
#include "Math/vec2.h"

namespace FlatFormer { namespace Collision {

	struct FF_AABB
	{	
		vec2 minCorner;	// bottom left corner
		vec2 maxCorner; // top right corner

		FF_AABB()
		{
			minCorner = vec2::Zero();
			maxCorner = vec2::Zero();
		}

		FF_AABB(vec2 r_min,vec2 r_max)
		{	
			minCorner = r_min;
			maxCorner = r_max;
		}

		FF_AABB(const FF_AABB & bound)
		{
			minCorner = bound.minCorner;
			maxCorner = bound.maxCorner;
		}


		bool isOverLap(const FF_AABB& other)
		{
			float d1x = other.minCorner.x - maxCorner.x;
			float d1y = other.minCorner.y - maxCorner.y;
			float d2x = minCorner.x - other.maxCorner.x;
			float d2y = minCorner.y - other.maxCorner.y;

			if (d1x > 0.0f || d1y > 0.0f)
				return false;

			if (d2x > 0.0f || d2y > 0.0f)
				return false;


			return true;
		}


		// returns true if the current aabb is completely
		// contained in another , otherwise false
		bool IsContain(const FF_AABB& other) const
		{
			bool result = true;

			result = result && minCorner.x <= other.minCorner.x;
			result = result && minCorner.y <= other.minCorner.y;
			result = result && other.maxCorner.x <= maxCorner.x;
			result = result && other.maxCorner.y <= maxCorner.y;

			return result;
		}


		bool operator == (const FF_AABB& other)
		{
			return this->maxCorner == other.maxCorner && this->minCorner == other.minCorner;
		}


		bool operator!=	(const FF_AABB& other)
		{
			return this->maxCorner != other.maxCorner || this->minCorner != other.minCorner;
		}


		bool operator == (const FF_AABB& other) const
		{
			return this->maxCorner == other.maxCorner && this->minCorner == other.minCorner;
		}


		bool operator!=	(const FF_AABB& other) const
		{
			return this->maxCorner != other.maxCorner || this->minCorner != other.minCorner;
		}

	};
}}