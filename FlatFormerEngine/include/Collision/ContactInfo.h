#pragma once

#include "Engine/API.h"
#include "ECS/ECS.h"
#include "Math/vec2.h"

namespace FlatFormer { namespace Collision {

	struct contactInfo
	{
		vec2 m_normal;
		vec2 m_penetrationVec;
		vec2 m_contactPoints[2];
		vec2 m_centroidFrom;
		vec2 m_centroidTo;

		ECS::EntityID entityIdFrom;
		ECS::EntityID entityIdTo;

		uint8 pointCount;
	};

}}