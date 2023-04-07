#pragma once

#include "ECS/Component.h"

#include "Asset/AssetHandle.h"
#include "Render/RenderStructs.h"
#include "Render/Sprite.h"

namespace FlatFormer {

	class SpriteComponent : public ECS::Component<SpriteComponent>
	{
	public:
		Sprite sprite;
		FlipType flipType;
		ColorRGBA color;

	public:
		SpriteComponent(Sprite sprite) :
			sprite(sprite),
			flipType(FlipType::None)
		{
		}

	};

}