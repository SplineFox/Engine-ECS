#pragma once

#include <SDL_image.h>

#include "Asset/IAssetLoader.h"
#include "Asset/Render/ImageAssetPayloadData.h"

namespace FlatFormer { namespace Asset {

	class JPGAssetLoader : public IAssetLoader
	{
		std::string GetPattern() override;
		bool IsUseRawFile() override;
		bool IsDiscardRawBufferAfterLoad() override;
		unsigned int GetLoadedResourceSize(char * rawBuffer, unsigned int rawSize) override;
		bool LoadResource(char * rawBuffer, unsigned int rawSize, std::shared_ptr<AssetHandle> handle) override;
	};

}}