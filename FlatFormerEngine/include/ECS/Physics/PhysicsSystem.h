#pragma once


#include "ECS/System.h"
#include "ECS/Common/TransformComponent.h"
#include "ECS/Physics/RigidBody/RigidBodyComponent.h"
#include "ECS/Collision/Colliders/ColliderComponent.h"
#include "ECS/Collision/CollisionEventComponent.h"

#include "Engine/Service/ComponentManagerUser.h"
#include "Engine/Service/EntityManagerUser.h"


namespace FlatFormer { namespace Physics {

	const double EPSILON = 0.0001f;

	struct ffe_manifold
	{
		RigidBodyComponent* m_bodyA;
		RigidBodyComponent* m_bodyB;

		ECS::TransformComponent* m_transformA;
		ECS::TransformComponent* m_transformB;

		vec2 m_centroidA;
		vec2 m_centroidB;

		vec2 m_penetrationVec;
		vec2 m_normal;

		vec2 m_contactPoints[2];
		uint8 m_contactCount;

		float m_e;
		float m_staticFriction;
		float m_dinamycFriction;
	};


	class PhysicsSystem :
		public ECS::System<PhysicsSystem>,
		public Service::FFE_SERV_USER_ComponentManager,
		public Service::FFE_SERV_USER_EntityManager
	{
#pragma region Using
		using RigidBodyTransformPair = std::pair<RigidBodyComponent*, ECS::TransformComponent*>;
		using RigidBodyVector = std::vector<RigidBodyTransformPair>;
		using ManifoldVector = std::vector<ffe_manifold>;
#pragma endregion

	private:
		RigidBodyVector m_rigidBodies;
		ManifoldVector	m_manifolds;

	public:
		PhysicsSystem();
		~PhysicsSystem();

		void PreUpdate(float deltaTime) override;

		void Update(float deltaTime) override;

		void PostUpdate(float deltaTime) override;

	private:

		void InitializeManifolds();

		void OnEntityAdded(ECS::EntityID entityId) override;

		void OnEntityRemoved(ECS::EntityID entityId) override;
	};

}}