#pragma once

#include "Engine/API.h"
#include "Engine/Service/MemoryManagerUser.h"

#include "Event/IEvent.h"
#include "Event/EventDispatcher.h"

#include "Memory/Allocator/QueueAllocator.h"

namespace FlatFormer { namespace Event {

	class EventManager : public Service::FFE_SERV_USER_MemoryManager
	{
	#pragma region MyRegion
		using EventDispatcherMap = std::unordered_map<EventTypeID, IEventDispatcher*>;
		using EventMemoryAllocator = Memory::Allocator::QueueAllocator;
		using EventBuffer = std::list<IEvent*>;
	#pragma endregion


	private:
		EventDispatcherMap		m_EventDispatcherMap;		// correspondence of event types and dispatchers responsible for their processing
		EventMemoryAllocator*	m_EventMemoryAllocator;		// used to allocate memory for events
									
		EventBuffer				m_EventBuffer[2];			// keeps a list of all dispatched events since the last call to EventHandler::DispatchEvents()
		int						m_ActiveBuffer;				// index of active buffer

		FFE_DECLARE_LOGGER

		EventManager(const EventManager&) = delete;
		EventManager& operator=(EventManager&) = delete;

	public:
		EventManager();
		~EventManager();


		// Registers a delegate by associating it with a dispatcher that handles an EventType.
		template<class EventType>
		void AddEventCallback(IEventDelegate* const eventDelegate)
		{
			EventTypeID ETID = EventType::STATIC_EVENT_TYPE_ID;

			/// check if there is a dispatcher handling the given EventTypeID
			EventDispatcherMap::const_iterator iterator = this->m_EventDispatcherMap.find(ETID);
			/// if the dispatcher is not found, then create it and add a delegate,
			if (iterator == this->m_EventDispatcherMap.end())
			{
				std::pair<EventTypeID, IEventDispatcher*> keyValuePair(ETID, new EventDispatcher<EventType>());

				keyValuePair.second->AddEventCallback(eventDelegate);

				this->m_EventDispatcherMap.insert(keyValuePair);
			}
			/// otherwise - add a delegate to an existing dispatcher
			else
			{
				this->m_EventDispatcherMap[ETID]->AddEventCallback(eventDelegate);
			}
		}


		// Удаляет делегат. 
		void RemoveEventCallback(IEventDelegate* eventDelegate)
		{
			auto typeID = eventDelegate->GetStaticEventTypeID();
			/// проверяем, существует ли диспетчер, обрабатывающий тип события,
			/// соответсвующего событию, обрабатывающемуся данным делегатом
			auto iterator = this->m_EventDispatcherMap.find(typeID);
			/// если существует, то отвязываем делегат от диспетчера
			if (iterator != this->m_EventDispatcherMap.end())
			{
				this->m_EventDispatcherMap[typeID]->RemoveEventCallback(eventDelegate);
			}
		}


		// Dispatches an event IMMEDIATELY.
		// A complete traversal of the queue is performed, calling all delegates that handle the EventType.
		// Almost the same as directly calling delegates from code.
		template<class EventType, class... ARGS>
		void SendEventByTrigger(ARGS&&... eventArgs)
		{
			//TODO: implement a call to Dispatch for all delegates handling an EventType (similar to 'Dispatch()')
		}


		// Sends an event to the queue.
		// When the next Update is called, all delegates will be called, provided there is enough time to process.
		template<class EventType, class... ARGS>
		void SendEventByQueue(ARGS&&... eventArgs)
		{
			/// check if the object type is trivially copyable
			static_assert(std::is_trivially_copyable<EventType>::value, "ASSERT: Event is not trivially copyable!");

			/// allocate memory to store the event
			void* eventPtr = this->m_EventMemoryAllocator->Allocate(sizeof(EventType), alignof(EventType));

			/// adding a new event to the buffer if the memory for the event object was allocated successfully
			if(eventPtr != nullptr)
			{
				m_EventBuffer[m_ActiveBuffer].push_back(new (eventPtr)EventType(std::forward<ARGS>(eventArgs)...));
				FFE_LOG_TRACE("'{}' event buffered", typeid(EventType).name());
			}
			else
			{
				FFE_LOG_WARNING("Event buffer is full! Cut off new incoming events!!!");
			}
		}


		// Processing the event queue.
		// Optionally has a processing time limit so that processing does not take too long.
		//NOTE: if there is not enough time, some events from the queue may not have time to be processed.
		void DispatchEvents(/*TODO: add time border*/)
		{
			/// swap buffers
			int bufferToProcess = m_ActiveBuffer;
			m_ActiveBuffer = (m_ActiveBuffer + 1) % 2;
			/// clear the working buffer
			m_EventBuffer[m_ActiveBuffer].clear();

			while (!m_EventBuffer[bufferToProcess].empty())
			{
				/// get the first element in the queue
				IEvent* event = this->m_EventBuffer[bufferToProcess].front();
				m_EventBuffer[bufferToProcess].pop_front();

				/// if the event object is corrupted,
				/// then skip the iteration
				if (event == nullptr)
				{
					FFE_LOG_ERROR("Skip corrupted event '{}'", event->GetEventTypeID());
					continue;
				}
				
				/// if an event of this type does not have a dispatcher,
				/// then skip the iteration
				auto iterator = this->m_EventDispatcherMap.find(event->GetEventTypeID());
				if (iterator == this->m_EventDispatcherMap.end())
				{
					m_EventMemoryAllocator->Deallocate(event);
					continue;
				}

				/// otherwise, we send it to the dispatcher for processing
				iterator->second->Dispatch(event);

				/// delete the event after it has been processed
				m_EventMemoryAllocator->Deallocate(event);

				/// TODO: check for timeout
				if (false /*condition if time is up*/)
				{
					FFE_LOG_WARNING("DispatchEvents: Aborting event processing! (Time ran out)")
					break;
				}
			}

			// while (if) the buffer is not empty,
			// transfer unhandled events from one buffer to another

			// note: to keep the sequence,
			// you need to add old events to the beginning of the active queue
			while (!m_EventBuffer[bufferToProcess].empty())
			{
				IEvent* event = this->m_EventBuffer[bufferToProcess].back();
				m_EventBuffer[bufferToProcess].pop_back();
				m_EventBuffer[m_ActiveBuffer].push_front(event);
			}
		}

		inline void ClearEventBuffer()
		{
			this->m_EventMemoryAllocator->Reset();
		}


		inline void ClearEventDispatcher()
		{
			this->m_EventDispatcherMap.clear();
		}
	};

}}