#include "ECS/IComponent.h"

namespace FlatFormer { namespace ECS {
	
	IComponent::IComponent() :
		m_OwnerID(INVALID_ENTITY_ID),
		m_IsActive(true)
	{}
	
	
	IComponent::~IComponent()
	{}
	
}}