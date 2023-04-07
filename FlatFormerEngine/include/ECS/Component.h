#pragma once

#include "ECS/IComponent.h"
#include "Util/TypeIDCounter.h"

namespace FlatFormer { namespace ECS {

	template<class ComponentType>
	class Component : public IComponent
	{
	public:
		static const ComponentTypeID STATIC_TYPE_ID;

	public:
		Component()
		{}

		virtual ~Component()
		{}

		inline ComponentTypeID GetStaticComponentTypeID() const { return STATIC_TYPE_ID; }
	};


	template<class ComponentType>
	const ComponentTypeID Component<ComponentType>::STATIC_TYPE_ID = Util::TypeIDCounter<IComponent>::Get<ComponentType>();
	
}}