#pragma once

#include "Engine/API.h"
#include "Event/IEventDispatcher.h"

namespace FlatFormer { namespace Event {

	template<class EventType>
	class EventDispatcher : public IEventDispatcher
	{
	#pragma region MyRegion
		using EventDelegateList = std::list<IEventDelegate*>;
	#pragma endregion

	private:
		EventDelegateList m_EventCallbacks;
		
		FFE_DECLARE_STATIC_LOGGER

	public:
		EventDispatcher()
		{}


		~EventDispatcher()
		{
			this->m_EventCallbacks.clear();
		}


		void Dispatch(IEvent* event) override
		{
			for (auto callback : this->m_EventCallbacks)
			{
				assert(callback != nullptr && "ASSERT: Invalid event callback");
				callback->Invoke(event);
			}
		}


		void AddEventCallback(IEventDelegate* const eventDelegate) override
		{
			/// check for the existence of the added delegate in the list of already registered ones
			auto result = std::find_if(	this->m_EventCallbacks.begin(),
										this->m_EventCallbacks.end(),
										[&](const IEventDelegate* other)
										{
											return other->operator==(eventDelegate);
										});

			/// if such a delegate is found, then we display a warning
			if (result != this->m_EventCallbacks.end())
			{
				FFE_LOG_WARNING("AddEventCallback: eventDelegate('{}') already exists and can't be added", eventDelegate->GetDelegateID())
			}
			/// if not found, then add to the list
			else
			{
				this->m_EventCallbacks.push_back(eventDelegate);
			}
		}


		void RemoveEventCallback(IEventDelegate* eventDelegate) override
		{
			/// check for the existence of the delegate to be removed in the list of already registered ones
			auto result = std::find_if(	this->m_EventCallbacks.begin(),
										this->m_EventCallbacks.end(),
										[&](const IEventDelegate* other)
										{
											return other->operator==(eventDelegate);
										});

			/// if found, then remove from the list
			if (result != this->m_EventCallbacks.end())
			{
				IEventDelegate* ptrMem = (IEventDelegate*)(*result);

				this->m_EventCallbacks.erase(result);
				delete ptrMem;
				ptrMem = nullptr;
			}
			/// if such a delegate is not found, then we display a warning
			else
			{
				FFE_LOG_WARNING("RemoveEventCallback: eventDelegate('{}') doesn't exists and can't be removed", eventDelegate->GetDelegateID());
			}
		}

		inline size_t GetEventCallbackCount() const override { return this->m_EventCallbacks.size(); }
	};

	FFE_DEFINE_STATIC_LOGGER_TEMPLATE(EventDispatcher, EventType, "EventDispatcher")

}}