#pragma once

#include "Event/Event.h"

namespace FlatFormer {

	struct LeftButtonDownEvent : public Event::Event<LeftButtonDownEvent>
	{
		int x;
		int y;

		LeftButtonDownEvent(int x, int y) : x(x), y(y)
		{}
	};


	struct LeftButtonUpEvent : public Event::Event<LeftButtonUpEvent>
	{
		int x;
		int y;

		LeftButtonUpEvent(int x, int y) : x(x), y(y)
		{}
	};


	struct LeftButtonPressedEvent : public Event::Event<LeftButtonPressedEvent>
	{
		int x;
		int y;

		LeftButtonPressedEvent(int x, int y) : x(x), y(y)
		{}
	};


	struct RightButtonDownEvent : public Event::Event<RightButtonDownEvent>
	{
		int x;
		int y;

		RightButtonDownEvent(int x, int y) : x(x), y(y)
		{}
	};


	struct RightButtonUpEvent : public Event::Event<RightButtonUpEvent>
	{
		int x;
		int y;

		RightButtonUpEvent(int x, int y) : x(x), y(y)
		{}
	};


	struct RightButtonPressedEvent : public Event::Event<RightButtonPressedEvent>
	{
		int x;
		int y;

		RightButtonPressedEvent(int x, int y) : x(x), y(y)
		{}
	};


	struct MouseMovedEvent : public Event::Event<MouseMovedEvent>
	{
		int x;
		int y;

		MouseMovedEvent(int x, int y) : x(x), y(y)
		{}
	};


	struct KeyDownEvent : public Event::Event<KeyDownEvent>
	{
		int keyCode;

		KeyDownEvent(int keyCode) : keyCode(keyCode)
		{}
	};


	struct KeyUpEvent : public Event::Event<KeyUpEvent>
	{
		int keyCode;

		KeyUpEvent(int keyCode) : keyCode(keyCode)
		{}
	};


	struct KeyPressedEvent : public Event::Event<KeyPressedEvent>
	{
		int keyCode;

		KeyPressedEvent(int keyCode) : keyCode(keyCode)
		{}
	};

}