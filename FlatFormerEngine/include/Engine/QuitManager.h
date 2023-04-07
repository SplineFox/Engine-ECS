#pragma once

#include "Engine/API.h"
#include "Event/IEventListener.h"

#include "Engine/GameEvents.h"
#include "Window/WindowEvents.h"
#include "Window/KeyCodes.h"
#include "Window/InputEvents.h"

namespace FlatFormer {

	class QuitManager : public Event::IEventListener
	{
	private:
		std::function<void()> m_QuitCallback;

	public:
		QuitManager()
		{
			this->RegisterEventCallback(&QuitManager::OnGameQuit);
			this->RegisterEventCallback(&QuitManager::OnWindowClosed);
			this->RegisterEventCallback(&QuitManager::OnECSPresed);
		}


		void SetQuitCallback(std::function<void()> callback)
		{
			m_QuitCallback = callback;
		}


	private:
		void OnWindowClosed(const WindowClosedEvent* event)
		{
			m_QuitCallback();
		}

		void OnGameQuit(const GameQuitEvent* event)
		{
			m_QuitCallback();
		}

		void OnECSPresed(const KeyDownEvent* event)
		{
			switch (event->keyCode)
			{
			case FFE_KEY_ESCAPE:
				m_QuitCallback();
				break;
			}
		}

	};
}
