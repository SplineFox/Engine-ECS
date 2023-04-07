#include "Engine/Engine.h"

#include "Engine/Service/EntityManagerUser.h"
#include "Engine/Service/ComponentManagerUser.h"
#include "Engine/Service/SystemManagerUser.h"
#include "Engine/Service/MemoryManagerUser.h"
#include "Engine/Service/EventManagerUser.h"
#include "Engine/Service/TickCounterUser.h"
#include "Engine/Service/IDPoolUser.h"
#include "Engine/Service/ViewUser.h"

#include "Engine/Service/AssetCacheUser.h"
#include "Asset/ResourceDirectory.h"
#include "Asset/ResourceZipFile.h"
#include "Asset/Asset.h"

#include "ECS/Render/SpriteComponent.h"
#include "ECS/Common/TransformComponent.h"
#include "ECS/Render/AnimatorComponent.h"
#include "ECS/Render/TextComponent.h"

namespace FlatFormer {

	Engine::Engine()
	{
		m_MemoryManager			= new Memory::MemoryManager();
		Service::MemoryManagerUser::ProvideMemoryManagerService(m_MemoryManager);

		m_EntityManager			= new ECS::EntityManager();
		Service::EntityManagerUser::ProvideEntityManagerService(m_EntityManager);

		m_ComponentManager		= new ECS::ComponentManager();
		Service::ComponentManagerUser::ProvideComponentManagerService(m_ComponentManager);

		m_SystemManager			= new ECS::SystemManager();
		Service::SystemManagerUser::ProvideSystemManagerService(m_SystemManager);
		
		m_EventManager			= new Event::EventManager();
		Service::EventManagerUser::ProvideEventManagerService(m_EventManager);

		m_TickCounter			= new Util::TickCounter();
		Service::TickCounterUser::ProvideTickCounterService(m_TickCounter);

		m_IDPool				= new Util::IDPool(10000);
		Service::IDPoolUser::ProvideIDPoolService(m_IDPool);

		m_View					= new Render::View(1280, 640);
		Service::ViewUser::ProvideViewService(m_View);

		m_QuitManager			= new QuitManager();


		Asset::IResourceProvider* provider = new Asset::ResourceDirectory();
		m_AssetCache = new Asset::AssetCache(10, provider);

		m_AssetCache->Init();

		Foo();
	}


	void Engine::Foo()
	{
		Asset::Asset asset("font.ttf");
		auto fontAssetHandle = m_AssetCache->GetHandle(&asset);

		Asset::Asset asset1("PNGAsset1.png");
		Asset::Asset asset2("PNGAsset2.png");
		auto imageAssetHandle1 = m_AssetCache->GetHandle(&asset1);
		auto imageAssetHandle2 = m_AssetCache->GetHandle(&asset2);

		Sprite spriteStruct1(imageAssetHandle1, 200, 200);
		Sprite spriteStruct2(imageAssetHandle2, 200, 200);

		Animation animation;
		animation.AddFrame(spriteStruct1, 0.5f);
		animation.AddFrame(spriteStruct2, 0.5f);

		auto entityID = m_EntityManager->CreateEntity();
		auto transform = m_ComponentManager->AddComponent<ECS::TransformComponent>(entityID);
		auto animator = m_ComponentManager->AddComponent<AnimatorComponent>(entityID, "testAnimation", animation);
		auto text = m_ComponentManager->AddComponent<TextComponent>(entityID, fontAssetHandle, "XUY", 25);

		transform->m_transform.TranslateAt(vec2(100, 100));
		transform->m_transform.RotateAt(25);
	}


	Engine::~Engine()
	{
		delete m_EntityManager;
		delete m_ComponentManager;
		delete m_SystemManager;
		delete m_EventManager;
		delete m_MemoryManager;
		delete m_TickCounter;
		delete m_IDPool;
		
		m_EntityManager		= nullptr;
		m_ComponentManager	= nullptr;
		m_SystemManager		= nullptr;
		m_EventManager		= nullptr;
		m_MemoryManager		= nullptr;
		m_TickCounter		= nullptr;
		m_IDPool			= nullptr;
	}


	void Engine::SetEngineQuitCallback(std::function<void()> callback)
	{
		m_QuitManager->SetQuitCallback(callback);
	}


	void Engine::Update(float deltaTime)
	{
		m_TickCounter->Tick(deltaTime);
		m_EntityManager->Update();
		m_SystemManager->Update(deltaTime);
		m_EntityManager->RemoveDestroyedEntities();
		m_EventManager->DispatchEvents();
	}

}