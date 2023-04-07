#pragma once

#include "Asset/AssetHandle.h"
#include "Render/RenderStructs.h"

namespace FlatFormer {

	class Sprite
	{
	public:
		Asset::SharedAssetHandle spriteHandle;
		int width, height;

	public:
		Sprite(Asset::SharedAssetHandle assetHandle, int width, int height) :
			spriteHandle(assetHandle),
			width(width),
			height(height)
		{
		}

	};

}