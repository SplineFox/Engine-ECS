#pragma once

#include "Window/IWindow.h"
#include "Window/IInput.h"

#include "Engine/Engine.h"
#include "Engine/Service/SystemManagerUser.h"

#include "ECS/Render/RenderBeginSystem.h"
#include "ECS/Render/BaseRenderSystem.h"
#include "ECS/Render/ColliderRenderSystem.h"
#include "ECS/Render/CollisionEventRenderSystem.h"
#include "ECS/Render/QuadTreeRenderSystem.h"
#include "ECS/Render/SpriteRenderSystem.h"
#include "ECS/Render/AnimationRenderSystem.h"
#include "ECS/Render/TextRenderSystem.h"
#include "ECS/Render/RenderEndSystem.h"

#include "ECS/Physics/PhysicsSystem.h"
#include "ECS/Physics/ForceIntegrationSystem.h"

#include "ECS/Collision/CollisionSystem.h"

#include "Util/Timer.h"

namespace FlatFormer
{
	class Application : public Service::FFE_SERV_USER_SystemManager
	{
	private:
		bool		m_IsRunning;

		IWindow*	m_Window;
		IInput*		m_Input;
		Engine*		m_Engine;
		Util::Timer m_timer;

	public:
		Application();
		~Application();

		void Run();

	private:
		void Initialize();
		void InitializeWindow();
		void InitializeEngine();
		void Shutdown();

		void OnEngineQuit();
	};

	// Must be overridden by the user (see "FlatFormer.h")
	Application* CreateApplication();
}


//	example:
//-------------------------------
//	[SandboxApp.cpp]
//-------------------------------
//	#include "FlatFormer.h"
//	
//	class Sandbox : public FlatFormer::Application
//	{
//	public:
//		Sandbox()
//		{
//			...
//		}
//	
//	
//		~Sandbox()
//		{
//			...
//		}
//	};
//	
//	FlatFormer::Application* FlatFormer::CreateApplication()
//	{
//		return new Sandbox;
//	}
//-------------------------------