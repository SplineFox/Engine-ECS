#include "Math/FFMath.h"

namespace FlatFormer {

	float ProjectOnNormal(vec2 point, vec2 n)
	{
		float	projection = vec2::Dot(point, n);

		return projection;
	};


	size_t ComputeNearestPointIndex(vec2 point, const std::vector<vec2>& points)
	{
		vec2 min(FFE_MAX_FLOAT, FFE_MAX_FLOAT);
		vec2 fromTo;
		size_t index = 0;

		for (size_t i = 0; i < points.size(); i++)
		{
			fromTo = points.at(i) - point;

			if (min.x*min.x + min.y*min.y > fromTo.x*fromTo.x + fromTo.y*fromTo.y)
			{
				min = fromTo;
				index = i;
			}
		}

		return index;
	};
}