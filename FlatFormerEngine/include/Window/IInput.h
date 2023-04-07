#pragma once

#include "Engine/API.h"
#include "Engine/Service/Wrap/EventManagerUserWrap.h"
#include "Window/InputEvents.h"

namespace FlatFormer {

	class IInput : public Service::EventManagerUserWrap
	{
	public:
		virtual void Update() = 0;

		static IInput* Create();
	};

}
