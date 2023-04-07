#pragma once

#include "ECS/System.h"
#include "ECS/Common/TransformComponent.h"
#include "ECS/Physics/RigidBody/RigidBodyComponent.h"

#include "Engine/Service/ComponentManagerUser.h"
#include "Engine/Service/EntityManagerUser.h"

namespace FlatFormer { namespace Physics {

	class ForceIntegrationSystem :
		public ECS::System<ForceIntegrationSystem>,
		public Service::FFE_SERV_USER_ComponentManager,
		public Service::FFE_SERV_USER_EntityManager
	{
#pragma region Using
		using RigidBodyTransformPair = std::pair<RigidBodyComponent*, ECS::TransformComponent*>;
		using RigidBodyVector = std::vector<RigidBodyTransformPair>;
#pragma endregion


	private:
		RigidBodyVector m_rigidBodies;

	public:

		ForceIntegrationSystem();
		~ForceIntegrationSystem();

		void PreUpdate(float deltaTime) override;

		void Update(float deltaTime) override;

		void PostUpdate(float deltaTime) override;

	private:

		void OnEntityAdded(ECS::EntityID entityId) override;

		void OnEntityRemoved(ECS::EntityID entityId) override;
	};
}}