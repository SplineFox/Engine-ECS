#pragma once

#include "Asset/IAssetLoader.h"

namespace FlatFormer { namespace Asset {

	class DefaultAssetLoader : public IAssetLoader
	{
	public:
		std::string GetPattern() override
		{
			return "*";
		}

		bool IsUseRawFile() override
		{
			return true;
		}

		bool IsDiscardRawBufferAfterLoad() override
		{
			return true;
		}

		unsigned int GetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) override
		{
			return rawSize;
		}

		bool LoadResource(char *rawBuffer, unsigned int rawSize, std::shared_ptr<AssetHandle> handle) override
		{
			return true;
		}
	};

}}