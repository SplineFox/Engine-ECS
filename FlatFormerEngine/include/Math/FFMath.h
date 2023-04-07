#pragma once

#include "Engine/API.h"

#include "Math/vec2.h"


namespace FlatFormer {

	// Search for the projection of a point onto a normal
	float	ProjectOnNormal(vec2 point , vec2 n);


	// Calculates the nearest point from the array
	size_t ComputeNearestPointIndex(vec2 point, const std::vector<vec2>& points);


	inline float ToRadians(float degrees) { return degrees * (FFE_PI / 180); };


	inline float ToDegrees(float radians) { return radians * (180 / FFE_PI); };


	// Gets the length of the intersection of the segments
	inline float IntersectLength(vec2 v1, vec2 v2)
	{
		double a = std::fmax(v1.x, v2.x);
		double b = std::fmin(v1.y, v2.y);

		double length = b - a;

		return float(length);
	};


	// Checks whether some segment contains point projection
	inline bool Contain(vec2 segment, float pointProj)
	{
		float min = FFE_FMINF(segment.x, segment.y);
		float max = FFE_FMAXF(segment.x, segment.y);

		if (min <= pointProj && max >= pointProj)
		{
			return true;
		}

		return false;
	};


	inline int FF_Signf(float a) 
	{
		if (a >= 0) 
		{
			return 1;
		}
		else 
		{
			return -1;
		}
	}


	inline int FF_Signi(int a)
	{
		if (a >= 0)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}


	inline int FF_Signd(double a)
	{
		if (a >= 0)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
}