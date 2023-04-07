#pragma once

#include "ECS/Component.h"

#include "Asset/AssetHandle.h"
#include "Asset/Render/FontAssetPayloadData.h"
#include "Render/RenderStructs.h"

#include <SDL.h>
#include <SDL_ttf.h>

namespace FlatFormer {

	class TextComponent : public ECS::Component<TextComponent>
	{
	public:
		Asset::SharedAssetHandle fontHandle;
		std::string text;
		int ptSize;
		ColorRGBA color;

	public:
		TextComponent(Asset::SharedAssetHandle assetHandle, std::string text, int ptSize) :
			fontHandle(assetHandle),
			text(text),
			ptSize(ptSize)
		{
		}

	};

}