#pragma once

#include "Event/IEventDelegate.h"

namespace FlatFormer { namespace Event {

	template<class Class, class EventType>
	class EventDelegate : public IEventDelegate
	{
		typedef void(Class::*CallbackFunction)(const EventType* const);

		Class* m_Receiver;
		CallbackFunction m_Callback;

	public:
		EventDelegate(Class* receiver, CallbackFunction& callback) :
			m_Receiver(receiver),
			m_Callback(callback)
		{}


		IEventDelegate* Clone() override
		{
			return new EventDelegate(this->m_Receiver, this->m_Callback);
		}


		inline void Invoke(const IEvent* const event) override
		{
			(m_Receiver->*m_Callback)(reinterpret_cast<const EventType* const>(event));
		}


		inline EventDelegateID GetDelegateID() const override
		{
			static const EventDelegateID DELEGATE_ID{ typeid(Class).hash_code() ^ typeid(CallbackFunction).hash_code() };
			return DELEGATE_ID;
		}


		inline TypeID GetStaticEventTypeID() const override
		{
			static const TypeID StaticEventTypeID { EventType::STATIC_EVENT_TYPE_ID };
			return StaticEventTypeID;
		}


		bool operator==(const IEventDelegate* other) const override
		{
			if (this->GetDelegateID() != other->GetDelegateID())
				return false;

			if (other == nullptr)
				return false;

			EventDelegate* otherDelegate = (EventDelegate*)other;
			return ((this->m_Receiver == otherDelegate->m_Receiver) && (this->m_Callback == otherDelegate->m_Callback));
		}
	};

}}