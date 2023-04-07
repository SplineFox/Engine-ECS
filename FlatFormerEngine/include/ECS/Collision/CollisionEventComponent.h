#pragma once

#include "ECS/Component.h"

#include "Math/vec2.h"

#include "Collision/ContactInfo.h"

namespace FlatFormer { namespace Collision {

	class  CollisionEventComponent: public ECS::Component<CollisionEventComponent>
	{
	public:
		contactInfo* m_contactInformation;

		CollisionEventComponent(contactInfo* contactInformation)
		{
			m_contactInformation = contactInformation;
		}

		~CollisionEventComponent()
		{
			m_contactInformation = nullptr;
		}

	private:

	};

}}