#pragma once

#include <SDL_ttf.h>

#include "Asset/IAssetLoader.h"
#include "Asset/Render/FontAssetPayloadData.h"

namespace FlatFormer {
	namespace Asset {

		class TTFAssetLoader : public IAssetLoader
		{
			std::string GetPattern() override;
			bool IsUseRawFile() override;
			bool IsDiscardRawBufferAfterLoad() override;
			unsigned int GetLoadedResourceSize(char * rawBuffer, unsigned int rawSize) override;
			bool LoadResource(char * rawBuffer, unsigned int rawSize, std::shared_ptr<AssetHandle> handle) override;
		};

	}
}