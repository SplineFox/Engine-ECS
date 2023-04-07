#pragma once

#include "Engine/API.h"

#include "ECS/Component.h"

#include "Math/vec2.h"

namespace FlatFormer { namespace Physics {

	struct bodyProperties
	{
		float m_density;
		float m_strength;
		float m_restitution;
		float m_staticFriction;
		float m_dynamicFriction;

		bodyProperties()
		{
			m_density			= 0.01f;
			m_strength			= FFE_DEFAULT_STRENGTH;
			m_restitution		= FFE_DEFAULT_RESTITUTION;
			m_staticFriction	= 0.4;
			m_dynamicFriction	= 0.1;
		}

		bodyProperties(const bodyProperties& other)
		{
			m_density			= other.m_density;
			m_strength			= other.m_strength;
			m_restitution		= other.m_restitution;
			m_staticFriction	= other.m_staticFriction;
			m_dynamicFriction	= other.m_dynamicFriction;
		}
	};

	class RigidBodyComponent : public ECS::Component<RigidBodyComponent>
	{

	private:

		float m_mass;
		float m_IMass;

		float m_inertia;
		float m_IInertia;

		vec2	m_force;
		vec2	m_pulse;

		float	m_torque;

		bool	m_isSleep;
	public:

		vec2	m_linearVelocity;
		float	m_angularVelocity;

		bodyProperties	m_properties;

		RigidBodyComponent();
		~RigidBodyComponent();

		inline float GetMass() { return m_mass; }

		inline float GetIMass() { return m_IMass; }

		inline float GetInertia() { return m_inertia; }

		inline float GetIInertia() { return m_IInertia; }

		inline vec2 GetForce() { return m_force; }	
		
		inline void AddForce(vec2 force) 
		{
			m_force += force;
		}

		inline void AddTorque(float torque)
		{
			m_torque += torque;
		}

		inline void ApplyPulse(vec2 pulse, vec2 contactVector)
		{
			m_linearVelocity += pulse * m_IMass;
			m_angularVelocity += vec2::Cross(contactVector, pulse) * m_IInertia;
		}

		inline void SetMass(float mass)
		{
			m_mass = mass;

			if (m_mass == 0)
			{
				m_IMass = 0;
				return;
			}

			m_IMass = 1 / m_mass;
		}

		inline void SetInertia(float inertia)
		{
			m_inertia = inertia;

			if (m_inertia == 0)
			{
				m_IInertia = 0;
				return;
			}

			m_IInertia = 1 / m_inertia;
		}

		inline void Update(float deltaTime)
		{
			m_linearVelocity += m_force * deltaTime * m_IMass;

			m_force.x = 0;
			m_force.y = 0;
		}
	};

}}