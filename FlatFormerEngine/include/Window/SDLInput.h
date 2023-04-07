#pragma once

#include "Window/IInput.h"
#include <SDL.h>

namespace FlatFormer {

	class SDLInput : public IInput
	{
	private:
		Uint8* m_PreKeyboardState;
	public:
		void Update() override;

	private:
		void ProcessSDLInputEvents();
	};

}