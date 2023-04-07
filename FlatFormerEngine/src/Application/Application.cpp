#include "Application/Application.h"

namespace FlatFormer
{
	Application::Application() :
		m_IsRunning(false)
	{
		Initialize();
	}


	Application::~Application()
	{}


	void Application::Initialize()
	{
		InitializeWindow();
		InitializeEngine();
	}


	void Application::Shutdown()
	{
		if (m_Window)
		{
			delete m_Window;
			m_Window = nullptr;
		}

		if (m_Input)
		{
			delete m_Input;
			m_Input = nullptr;
		}

		if (m_Engine)
		{
			delete m_Engine;
			m_Engine = nullptr;
		}
	}


	void Application::InitializeWindow()
	{
		m_Window = IWindow::Create();
		m_Input = IInput::Create();
	}


	void Application::InitializeEngine()
	{
		m_Engine = new Engine();

		if (m_Engine)
		{
			ECS::SystemManager& systemManager = Service::SystemManagerUser::GetSystemManagerService();
			
			systemManager.AddSystem<RenderBeginSystem>(m_Window->GetContext());
				systemManager.AddSystem<BaseRenderSystem>(m_Window->GetContext());
				systemManager.AddSystem<SpriteRenderSystem>(m_Window->GetContext());
				systemManager.AddSystem<AnimationRenderSystem>(m_Window->GetContext());
				systemManager.AddSystem<TextRenderSystem>(m_Window->GetContext());
			systemManager.AddSystem<RenderEndSystem>(m_Window->GetContext());

			systemManager.AddSystem<Physics::ForceIntegrationSystem>();

			systemManager.AddSystem<Collision::CollisionSystem>();	

			systemManager.AddSystem<Physics::PhysicsSystem>();

			std::function<void()> onEngineQuit;
			onEngineQuit = std::bind(&Application::OnEngineQuit, this);
			m_Engine->SetEngineQuitCallback(onEngineQuit);
		}
	}


	void Application::OnEngineQuit()
	{
		m_IsRunning = false;
	}


	void Application::Run()
	{
		m_IsRunning = true;

		while (m_IsRunning)
		{
			m_timer.Tick();
			if (m_timer.GetDeltaTime() >= 1.0f / 60.0f)
			{	
				m_timer.Reset();	
				m_Window->Update();
				m_Input->Update();
				m_Engine->Update(0.01f);
			}
		}
	}
}