#pragma once

#include "Event/Event.h"

namespace FlatFormer {

	struct GameQuitEvent : Event::Event<GameQuitEvent>
	{};

}