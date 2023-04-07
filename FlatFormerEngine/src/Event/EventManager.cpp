#include "Event/EventManager.h"

namespace FlatFormer { namespace Event {

	EventManager::EventManager()
	{
		FFE_DEFINE_LOGGER("EventManager")

		m_EventMemoryAllocator = new EventMemoryAllocator(EVENT_MEMORY_BUFFER_SIZE, this->GetMemoryManagerService().Allocate(EVENT_MEMORY_BUFFER_SIZE, "EventManager"));
		// memory reservation for storing elements, is used to minimize additional extension
		// m_EventBuffer.reserve(1024);
		// TODO: implement memory reservation for buffers

		m_ActiveBuffer = 0;

		FFE_LOG_INFO("Initialized")
	}


	EventManager::~EventManager()
	{
		for (EventDispatcherMap::iterator iterator = this->m_EventDispatcherMap.begin(); iterator != this->m_EventDispatcherMap.end(); ++iterator)
		{
			delete (*iterator).second;
			(*iterator).second = nullptr;
		}
		m_EventDispatcherMap.clear();

		this->GetMemoryManagerService().Deallocate((void*)m_EventMemoryAllocator->GetMemoryAddress0());
		delete m_EventMemoryAllocator;
		m_EventMemoryAllocator = nullptr;

		FFE_LOG_INFO("Released");
	}

}}