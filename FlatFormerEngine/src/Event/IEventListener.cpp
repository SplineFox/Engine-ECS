#include "Event/IEventListener.h"

namespace FlatFormer { namespace Event {

	IEventListener::IEventListener()
	{}


	IEventListener::~IEventListener()
	{
		UnregisterAllEventCallbacks();
	}


	void IEventListener::UnregisterAllEventCallbacks()
	{
		for (auto delegate : this->m_RegisteredCallbacks)
		{
			this->UnsubscribeEvent(delegate);
		}
		this->m_RegisteredCallbacks.clear();
	}

}}