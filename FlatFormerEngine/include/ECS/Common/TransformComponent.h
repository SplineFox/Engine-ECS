#pragma once

#include "ECS/Component.h"

#include "Math/Transform2D.h"

namespace FlatFormer { namespace ECS {

	class TransformComponent : public Component<TransformComponent>
	{
	public:
		Transform2D m_transform;
	};

}}