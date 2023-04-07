#pragma once

#include "ECS/Component.h"
#include "ECS/Collision/Colliders/Collider.h"

#include "Engine/API.h"
#include "Engine/Platform.h"


#include "Math/vec2.h"
#include "Math/Transform2D.h"


#include "Collision/FF_AABB.h"


namespace FlatFormer{ namespace Collision{

	class ColliderComponent:public FlatFormer::ECS::Component<ColliderComponent>
	{
	public:
		Collider* m_collider;

		ColliderComponent(Collider* collider)
		{
			m_collider = collider;
		}

		~ColliderComponent()
		{
			m_collider = nullptr;
		}
	};
}}
