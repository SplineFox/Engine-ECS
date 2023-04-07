#include "ECS/Physics/ForceIntegrationSystem.h"

namespace FlatFormer {namespace Physics {

	ForceIntegrationSystem::ForceIntegrationSystem()
	{

		this->m_SystemFilter.SetItemByType<RigidBodyComponent>(true);
		this->m_SystemFilter.SetItemByType<ECS::TransformComponent>(true);

		m_rigidBodies.reserve(100);
	}

	ForceIntegrationSystem::~ForceIntegrationSystem()
	{
		m_rigidBodies.clear();
	}

	void ForceIntegrationSystem::PreUpdate(float deltaTime)
	{
		for (size_t i = 0; i < m_rigidBodies.size(); i++)
		{
			if (m_rigidBodies[i].first->GetMass() == 0)
			{
				continue;
			}

			m_rigidBodies[i].first->AddForce(vec2(0, FFE_GRAVITY*50)*m_rigidBodies[i].first->GetMass());
		}
	}

	void ForceIntegrationSystem::Update(float deltaTime)
	{}

	void ForceIntegrationSystem::PostUpdate(float deltaTime) 
	{
		for (size_t i = 0; i < m_rigidBodies.size(); i++)
		{
			m_rigidBodies[i].first->Update(deltaTime);
			m_rigidBodies[i].second->m_transform.Translate(m_rigidBodies[i].first->m_linearVelocity* deltaTime);
			m_rigidBodies[i].second->m_transform.Rotate(-ToDegrees(m_rigidBodies[i].first->m_angularVelocity* deltaTime));
		}
	}

	void ForceIntegrationSystem::OnEntityAdded(ECS::EntityID entityId)
	{
		RigidBodyComponent* rigidBody = this->GetComponentManagerService().GetComponent<RigidBodyComponent>(entityId);
		ECS::TransformComponent* transform = this->GetComponentManagerService().GetComponent<ECS::TransformComponent>(entityId);

		m_rigidBodies.emplace_back(RigidBodyTransformPair(rigidBody, transform));

		rigidBody = nullptr;
		transform = nullptr;
	}

	void ForceIntegrationSystem::OnEntityRemoved(ECS::EntityID entityId)
	{
		RigidBodyComponent* rigidBody = this->GetComponentManagerService().GetComponent<RigidBodyComponent>(entityId);
		ECS::TransformComponent* transform = this->GetComponentManagerService().GetComponent<ECS::TransformComponent>(entityId);

		m_rigidBodies.erase(std::remove(m_rigidBodies.begin(), m_rigidBodies.end(), RigidBodyTransformPair(rigidBody, transform)), m_rigidBodies.end());

		rigidBody = nullptr;
		transform = nullptr;
	}

}}