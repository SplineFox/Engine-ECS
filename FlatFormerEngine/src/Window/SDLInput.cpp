#include "Window/SDLInput.h"

namespace FlatFormer {

	IInput* IInput::Create()
	{
		return new SDLInput();
	}


	void SDLInput::Update()
	{
		ProcessSDLInputEvents();
	}


	void SDLInput::ProcessSDLInputEvents()
	{
		SDL_Event event;

		// only handle keyboard and mouse events
		while (SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_KEYDOWN, SDL_MOUSEWHEEL))
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
				{
					SendEventByQueue<KeyDownEvent>(event.key.keysym.scancode);
					break;
				}

				case SDL_KEYUP:
					SendEventByQueue<KeyUpEvent>(event.key.keysym.scancode);
					break;


				case SDL_MOUSEBUTTONDOWN:
				{
					switch (event.button.button)
					{
					case SDL_BUTTON_LEFT:
						SendEventByQueue<LeftButtonDownEvent>(event.motion.x, event.motion.y);
						break;

					case SDL_BUTTON_RIGHT:
						SendEventByQueue<RightButtonDownEvent>(event.motion.x, event.motion.y);
						break;
					}
					break;
				}

				case SDL_MOUSEBUTTONUP:
				{
					switch (event.button.button)
					{
					case SDL_BUTTON_LEFT:
						SendEventByQueue<LeftButtonUpEvent>(event.motion.x, event.motion.y);
						break;

					case SDL_BUTTON_RIGHT:
						SendEventByQueue<RightButtonUpEvent>(event.motion.x, event.motion.y);
						break;
					}
					break;
				}

				case SDL_MOUSEMOTION:
				{
					SendEventByQueue<MouseMovedEvent>(event.motion.x, event.motion.y);
				}
			}
		}
	}

}