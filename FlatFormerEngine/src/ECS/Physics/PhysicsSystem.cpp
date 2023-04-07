#include"ECS/Physics/PhysicsSystem.h"


namespace FlatFormer { namespace Physics {

	PhysicsSystem::PhysicsSystem()
	{
		this->m_SystemFilter.SetItemByType<RigidBodyComponent>(true);
		this->m_SystemFilter.SetItemByType<ECS::TransformComponent>(true);
		
		m_rigidBodies.reserve(100); // todo: awoid magic numbers
		m_manifolds.reserve(100); // todo: awoid magic numbers
	}

	PhysicsSystem::~PhysicsSystem()
	{
		m_rigidBodies.clear();
		m_manifolds.clear();
	}

	void PhysicsSystem::InitializeManifolds()
	{
		auto& collisions = this->GetComponentManagerService().GetComponentsOfType<FlatFormer::Collision::CollisionEventComponent>();

		ffe_manifold manifold;

		RigidBodyComponent * rb1;
		RigidBodyComponent * rb2;

		ECS::TransformComponent* transform1;
		ECS::TransformComponent* transform2;

		for (size_t i = 0; i < collisions.size(); i++)
		{
			rb1 = this->GetComponentManagerService().GetComponent<RigidBodyComponent>(collisions[i]->m_contactInformation->entityIdFrom);
			rb2 = this->GetComponentManagerService().GetComponent<RigidBodyComponent>(collisions[i]->m_contactInformation->entityIdTo);

			transform1 = this->GetComponentManagerService().GetComponent<ECS::TransformComponent>(collisions[i]->m_contactInformation->entityIdFrom);
			transform2 = this->GetComponentManagerService().GetComponent<ECS::TransformComponent>(collisions[i]->m_contactInformation->entityIdTo);

			if (rb1 == nullptr || rb2 == nullptr)
			{
				continue;
			}

			manifold.m_bodyA = rb1;
			manifold.m_bodyB = rb2;

			manifold.m_transformA = transform1;
			manifold.m_transformB = transform2;

			manifold.m_centroidA = collisions[i]->m_contactInformation->m_centroidFrom;
			manifold.m_centroidB = collisions[i]->m_contactInformation->m_centroidTo;

			manifold.m_penetrationVec = collisions[i]->m_contactInformation->m_penetrationVec;
			manifold.m_normal = collisions[i]->m_contactInformation->m_normal;

			manifold.m_contactPoints[0] = collisions[i]->m_contactInformation->m_contactPoints[0];
			manifold.m_contactPoints[1] = collisions[i]->m_contactInformation->m_contactPoints[1];

			manifold.m_contactCount = collisions[i]->m_contactInformation->pointCount;

			manifold.m_e = FFE_FMAXF(rb1->m_properties.m_restitution, rb2->m_properties.m_restitution);

			manifold.m_staticFriction = std::sqrt(rb1->m_properties.m_staticFriction *rb1->m_properties.m_staticFriction);
			manifold.m_dinamycFriction = std::sqrt(rb1->m_properties.m_dynamicFriction *rb1->m_properties.m_dynamicFriction);

			m_manifolds.emplace_back(manifold);
		}

		rb1 = nullptr;
		rb2 = nullptr;
		transform1 = nullptr;
		transform2 = nullptr;
	}

	void PhysicsSystem::PreUpdate(float deltaTime)
	{}
	
	void PhysicsSystem::Update(float deltaTime)
	{
		InitializeManifolds();

		for (size_t i = 0; i < m_manifolds.size(); i++)
		{
			if (m_manifolds[i].m_bodyA->GetMass() + m_manifolds[i].m_bodyB->GetMass() == 0)
			{
				continue;
			}

		    double k_slop = 0.05f; // Penetration allowance
			double percent = 0.4f; // Penetration percentage to correct

			vec2 correction = (std::max(m_manifolds[i].m_penetrationVec.Length() - k_slop, 0.0) / (m_manifolds[i].m_bodyA->GetIMass() + m_manifolds[i].m_bodyB->GetIMass())) * percent * m_manifolds[i].m_normal;


			m_manifolds[i].m_transformA->m_transform.Translate(-m_manifolds[i].m_bodyA->GetIMass() * correction);
			m_manifolds[i].m_transformB->m_transform.Translate(m_manifolds[i].m_bodyB->GetIMass() * correction);

			for (size_t u = 0; u < 20; u++)
			{
				for (uint32 k = 0; k < m_manifolds[i].m_contactCount; ++k)
				{
					// Calculate radii from COM to contact
					vec2 ra = m_manifolds[i].m_contactPoints[k] - m_manifolds[i].m_centroidA;
					vec2 rb = m_manifolds[i].m_contactPoints[k] - m_manifolds[i].m_centroidB;
					rb -= m_manifolds[i].m_penetrationVec;


					// Relative velocity
					vec2 rv = m_manifolds[i].m_bodyB->m_linearVelocity + vec2::Cross(m_manifolds[i].m_bodyB->m_angularVelocity, rb) -
						m_manifolds[i].m_bodyA->m_linearVelocity - vec2::Cross(m_manifolds[i].m_bodyA->m_angularVelocity, ra);

					// Relative velocity along the normal
					double contactVel = vec2::Dot(rv, m_manifolds[i].m_normal);

					// Do not resolve if velocities are separating
					if (contactVel > 0)
						continue;

					double raCrossN = vec2::Cross(ra, m_manifolds[i].m_normal);
					double rbCrossN = vec2::Cross(rb, m_manifolds[i].m_normal);


					double invMassSum =
						m_manifolds[i].m_bodyA->GetIMass() +
						m_manifolds[i].m_bodyB->GetIMass() +
						(raCrossN* raCrossN) * m_manifolds[i].m_bodyA->GetIInertia() +
						(rbCrossN* rbCrossN) * m_manifolds[i].m_bodyB->GetIInertia();

					// Calculate impulse scalar
					double j = -(1.0f + m_manifolds[i].m_e) * contactVel;

					j /= invMassSum;
					j /= (double)m_manifolds[i].m_contactCount;

					vec2 impulse = j * m_manifolds[i].m_normal;

					// Apply impulse

					m_manifolds[i].m_bodyA->ApplyPulse(-impulse,ra);
					m_manifolds[i].m_bodyB->ApplyPulse(impulse, rb);

					rv = m_manifolds[i].m_bodyB->m_linearVelocity + vec2::Cross(m_manifolds[i].m_bodyB->m_angularVelocity, rb) -
						m_manifolds[i].m_bodyA->m_linearVelocity - vec2::Cross(m_manifolds[i].m_bodyA->m_angularVelocity, ra);

					vec2 t = rv - (m_manifolds[i].m_normal * vec2::Dot(rv, m_manifolds[i].m_normal));
					t = vec2::Normalize(t);


					// j tangent magnitude
					double jt = -vec2::Dot(rv, t);
					jt /= invMassSum;
					jt /= (double)m_manifolds[i].m_contactCount;

					// Don't apply tiny friction impulses
					if (jt != 0.0f)
					{
						// Coulumb's law
						vec2 tangentImpulse;
						if (std::abs(jt) < j * m_manifolds[i].m_staticFriction)
							tangentImpulse = t * jt;
						else
							tangentImpulse = t * -j * m_manifolds[i].m_dinamycFriction;

						// Apply friction impulse

						/*m_manifolds[i].m_bodyA->m_linearVelocity += -tangentImpulse * m_manifolds[i].m_bodyA->GetIMass();
						m_manifolds[i].m_bodyB->m_linearVelocity += tangentImpulse * m_manifolds[i].m_bodyB->GetIMass();

						m_manifolds[i].m_bodyA->m_angularVelocity += vec2::Cross(ra, -tangentImpulse) * m_manifolds[i].m_bodyA->GetIInertia();
						m_manifolds[i].m_bodyB->m_angularVelocity += vec2::Cross(rb, tangentImpulse) * m_manifolds[i].m_bodyB->GetIInertia();*/

						m_manifolds[i].m_bodyA->ApplyPulse(-tangentImpulse, ra);
						m_manifolds[i].m_bodyB->ApplyPulse(tangentImpulse, rb);
					}
				}
			}
		}

		m_manifolds.clear();
	}

	void PhysicsSystem::PostUpdate(float deltaTime)
	{}

	void PhysicsSystem::OnEntityAdded(ECS::EntityID entityId)
	{
		RigidBodyComponent* rigidBody = this->GetComponentManagerService().GetComponent<RigidBodyComponent>(entityId);
		ECS::TransformComponent* transform = this->GetComponentManagerService().GetComponent<ECS::TransformComponent>(entityId);

		m_rigidBodies.emplace_back(RigidBodyTransformPair(rigidBody, transform));

		rigidBody = nullptr;
		transform = nullptr;
	}

	void PhysicsSystem::OnEntityRemoved(ECS::EntityID entityId)
	{
		RigidBodyComponent* rigidBody = this->GetComponentManagerService().GetComponent<RigidBodyComponent>(entityId);
		ECS::TransformComponent* transform = this->GetComponentManagerService().GetComponent<ECS::TransformComponent>(entityId);

		m_rigidBodies.erase(std::remove(m_rigidBodies.begin(), m_rigidBodies.end(), RigidBodyTransformPair(rigidBody,transform)), m_rigidBodies.end());

		rigidBody = nullptr;
		transform = nullptr;
	}
}}