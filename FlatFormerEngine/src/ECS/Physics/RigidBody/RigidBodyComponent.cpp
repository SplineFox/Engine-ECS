#include"ECS/Physics/RigidBody/RigidBodyComponent.h"

namespace FlatFormer { namespace Physics {

	RigidBodyComponent::RigidBodyComponent()
	{
		m_mass = 0;
		m_IMass = 0;
		m_inertia = 0;
		m_IInertia = 0;
		m_angularVelocity = 0;
		m_linearVelocity = vec2();
		m_properties = bodyProperties();

		m_force = vec2();
		m_torque = 0;
	}

	RigidBodyComponent::~RigidBodyComponent()
	{}


}}