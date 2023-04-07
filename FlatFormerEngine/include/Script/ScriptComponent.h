#pragma once

#include <iostream>
#include "ECS/Component.h"

namespace FlatFormer {

	class ScriptComponent : public ECS::Component<ScriptComponent>
	{
	public:
		std::string source;

	public:
		ScriptComponent(std::string source)
		{
			std::cout << "ScriptComponent: CREATED" << std::endl;
			this->source = source;
		}

		~ScriptComponent()
		{
			std::cout << "ScriptComponent: DELETED" << std::endl;
		}

	};

}