#pragma once

#include "Util/ID.h"

namespace FlatFormer { namespace ECS {

#pragma region Using
	using EntityTypeID = TypeID;
	using ComponentTypeID = TypeID;
	using SystemTypeID = TypeID;

	using EntityID = Util::ID;
	using ComponentID = Util::ID;
#pragma endregion


	static const EntityID		INVALID_ENTITY_ID		= Util::ID::INVALID_ID;
	static const ComponentID	INVALID_COMPONENT_ID	= Util::ID::INVALID_ID;
	static const SystemTypeID	INVALID_SYSTEM_ID		= INVALID_TYPE_ID;

}}