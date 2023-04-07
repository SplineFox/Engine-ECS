#pragma once

#include "Engine/API.h"
#include "Engine/QuitManager.h"

#include "ECS/Collision/Colliders/ColliderComponent.h"
#include "ECS/Collision/Colliders/CircleCollider.h"
#include "ECS/Collision/Colliders/PolygonCollider.h"
#include "ECS/Common/TransformComponent.h"
#include "ECS/Physics/RigidBody/RigidBodyComponent.h"

namespace FlatFormer {

#pragma region Forward declaration
	namespace Asset
	{
		class AssetCache;
	}

	namespace ECS
	{
		class EntityManager;
		class ComponentManager;
		class SystemManager;
	}

	namespace Memory
	{
		class MemoryManager;
	}

	namespace Event
	{
		class EventManager;
	}

	namespace Util
	{
		class IDPool;
		class TickCounter;
	}

	namespace Render
	{
		class View;
	}
#pragma endregion


	class Engine
	{
	private:
		Asset::AssetCache*		m_AssetCache;

		ECS::EntityManager*		m_EntityManager;
		ECS::ComponentManager*	m_ComponentManager;
		ECS::SystemManager*		m_SystemManager;
		
		Memory::MemoryManager*	m_MemoryManager;
		Event::EventManager*	m_EventManager;
		Util::TickCounter*		m_TickCounter;
		Util::IDPool*			m_IDPool;

		Render::View*			m_View;

		QuitManager*			m_QuitManager;

		Engine(const Engine&)		= delete;
		Engine& operator=(Engine&)	= delete;


	public:
		Engine();
		~Engine();

		void SetEngineQuitCallback(std::function<void()> callback);

		void Foo();
		void Update(float deltaTime);
	};

}