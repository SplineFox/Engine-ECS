#pragma once

#include "ECS/Component.h"

#include "Math/vec2.h"

#include "Collision/QuadTree/QuadTree.h"


namespace FlatFormer {
	namespace Collision {

		class  QuadTreeDebugComponent : public ECS::Component<QuadTreeDebugComponent>
		{
		public:
			QuadTree* m_quadTree;

			QuadTreeDebugComponent(QuadTree* quadTree)
			{
				m_quadTree = quadTree;
			}

			~QuadTreeDebugComponent()
			{
				m_quadTree = nullptr;
			}
		};

	}
}