#pragma once

#include "ECS/ISystem.h"
#include "Util/TypeIDCounter.h"

namespace FlatFormer { namespace ECS {

	template<class SystemType>
	class System : public ISystem
	{
	protected:
		FFE_DECLARE_LOGGER

	public:
		static const SystemTypeID STATIC_TYPE_ID;

	protected:
		System()
		{
			FFE_DEFINE_LOGGER(typeid(SystemType).name())
			FFE_LOG_INFO("Initialized")
		}

	public:
		virtual ~System()
		{
			FFE_LOG_INFO("Released");
		}


		inline const SystemTypeID GetStaticSystemTypeID() const override
		{
			return STATIC_TYPE_ID;
		}


		inline const char* GetSystemTypeName() const override
		{
			static const char* SYSTEM_TYPE_NAME{ typeid(SystemType).name() };
			return SYSTEM_TYPE_NAME;
		}


		virtual void Initialize() override
		{}


		virtual void PreUpdate(float deltaTime) override
		{}


		virtual void Update(float deltaTime) override
		{}


		virtual void PostUpdate(float deltaTime) override
		{}

	};


	template<class SystemType>
	const SystemTypeID System<SystemType>::STATIC_TYPE_ID = Util::TypeIDCounter<ISystem>::Get<SystemType>();

}}