#pragma once

#include "ECS/System.h"

#include "ECS/Collision/Colliders/CircleCollider.h"
#include "ECS/Collision/Colliders/PolygonCollider.h"
#include "ECS/Collision/Colliders/ColliderComponent.h"
#include "ECS/Common/TransformComponent.h"

#include "ECS/Collision/CollisionEventComponent.h"

#include "Engine/Service/ComponentManagerUser.h"
#include "Engine/Service/EntityManagerUser.h"

#include "Collision/BroadPhase.h"
#include "Collision/ContactInfo.h"

#include "Math/FFMath.h"


namespace FlatFormer { namespace Collision {


	class CollisionSystem : 
		public ECS::System<CollisionSystem>,
		public Service::FFE_SERV_USER_ComponentManager,
		public Service::FFE_SERV_USER_EntityManager
	{
#pragma region Using
		using uint8 = FlatFormer::uint8;
		using ColliderEntityPair = std::pair<ColliderComponent*, ECS::EntityID>;
		using ColliderVector = std::vector<std::pair<ColliderEntityPair, ECS::TransformComponent*>>;
		using ContactVector = std::vector<contactInfo>;
		using vec2Array = std::vector<vec2>;
#pragma endregion


	private:
		ColliderVector	m_colliders;		// list of pairs : collider, transform, - entity id
		ContactVector	m_contactRegister;	// list of registered contacts
		BroadPhase *	m_braodPhase;

	public:

		CollisionSystem();
		~CollisionSystem();


		//CircleCollider VS CircleCollider
		void Collide
		(
			const CircleCollider* circleA,
			ECS::EntityID entityFrom,
			const CircleCollider* circleB,
			ECS::EntityID entityTo
		);


		//PolygonCollider VS CircleCollider
		void Collide
		(
			const PolygonCollider* polygon,
			ECS::EntityID entityFrom,
			const CircleCollider* circle,
			ECS::EntityID entityTo
		);


		//PolygonCollider VS PolygonCollider
		void Collide
		(
			const PolygonCollider* polygonA,
			ECS::EntityID entityFrom,
			const PolygonCollider* polygonB,
			ECS::EntityID entityTo
		);


		//ColliderComponent VS ColliderComponent
		void Collide
		(
			const ColliderComponent* colliderA,
			ECS::EntityID entityFrom,
			const ColliderComponent* colliderB,
			ECS::EntityID entityTo
		);


		void PreUpdate(float deltaTime) override;

		void Update(float deltaTime) override;

		void PostUpdate(float deltaTime) override;


	private:

		void OnEntityAdded(ECS::EntityID entityId) override;

		void OnEntityRemoved(ECS::EntityID entityId) override;
	};
}}