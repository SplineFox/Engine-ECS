#pragma once

#include "Event/Event.h"

namespace FlatFormer {

	struct WindowMinimizedEvent : Event::Event<WindowMinimizedEvent>
	{};


	struct WindowMaximizedEvent : Event::Event<WindowMaximizedEvent>
	{};


	struct WindowResizedEvent : Event::Event<WindowResizedEvent>
	{
		int width;
		int height;

		WindowResizedEvent(int width, int height) :
			width(width),
			height(height)
		{}
	};


	struct WindowRestoredEvent : Event::Event<WindowRestoredEvent>
	{};


	struct WindowClosedEvent : Event::Event<WindowClosedEvent>
	{};
}