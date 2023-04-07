#pragma once

#include "Engine/API.h"
#include "Engine/Service/Wrap/EventManagerUserWrap.h"

namespace FlatFormer { namespace Event {

	class IEventListener : public Service::FFE_SERV_USER_WRAP_EventManager
	{
	#pragma region Using
		using RegisteredCallbacks = std::list<IEventDelegate*>;
	#pragma endregion

	private:
		RegisteredCallbacks m_RegisteredCallbacks;

	public:
		IEventListener();
		virtual ~IEventListener();


		template<class Class, class EventType>
		void RegisterEventCallback(void(Class::* Callback)(const EventType * const))
		{
			IEventDelegate* eventDelegate = new EventDelegate<Class, EventType>(static_cast<Class*>(this), Callback);

			m_RegisteredCallbacks.push_back(eventDelegate);
			this->SubscribeEvent<EventType>(eventDelegate);
		}


		template<class Class, class EventType>
		void UnregisterEventCallback(void(Class::*Callback)(const EventType* const))
		{
			EventDelegate<Class, EventType> eventDelegate(static_cast<Class*>(this), Callback);

			for (auto callback : this->m_RegisteredCallbacks)
			{
				if (callback->GetDelegateID() == eventDelegate.GetDelegateID())
				{
					m_RegisteredCallbacks.remove_if(
						[&](const IEventDelegate* other)
						{
							return other->operator==(eventDelegate);
						});

					this->UnsubscribeEvent(&eventDelegate);
					break;
				}
			}
		}

		void UnregisterAllEventCallbacks();
	};

}}