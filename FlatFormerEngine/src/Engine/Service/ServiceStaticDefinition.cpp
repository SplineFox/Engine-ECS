#include "Engine/Service/EntityManagerUser.h"
#include "Engine/Service/ComponentManagerUser.h"
#include "Engine/Service/SystemManagerUser.h"
#include "Engine/Service/MemoryManagerUser.h"
#include "Engine/Service/EventManagerUser.h"
#include "Engine/Service/TickCounterUser.h"
#include "Engine/Service/IDPoolUser.h"
#include "Engine/Service/ViewUser.h"
#include "Engine/Service/AssetCacheUser.h"

namespace FlatFormer { namespace Service {

	ECS::EntityManager* EntityManagerUser::s_EntityManagerService;

	ECS::ComponentManager* ComponentManagerUser::s_ComponentManagerService;

	ECS::SystemManager* SystemManagerUser::s_SystemManagerService;

	Memory::MemoryManager* MemoryManagerUser::s_MemoryManagerService;

	Event::EventManager* EventManagerUser::s_EventManagerService;

	Util::TickCounter* TickCounterUser::s_TickCounterService;

	Util::IDPool* IDPoolUser::s_IDPoolService;

	Render::View* ViewUser::s_ViewService;

	Asset::AssetCache* AssetCacheUser::s_AssetCacheService;

}}