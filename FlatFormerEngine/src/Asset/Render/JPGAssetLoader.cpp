#include "Asset/Render/JPGAssetLoader.h"

#include "Asset/AssetHandle.h"

namespace FlatFormer { namespace Asset {

	std::string JPGAssetLoader::GetPattern()
	{
		return "*.jpg";
	}


	bool JPGAssetLoader::IsUseRawFile()
	{
		return false;
	}


	bool JPGAssetLoader::IsDiscardRawBufferAfterLoad()
	{
		return true;
	}


	unsigned int JPGAssetLoader::GetLoadedResourceSize(char* rawBuffer, unsigned int rawSize)
	{
		return rawSize;
	}


	bool JPGAssetLoader::LoadResource(char* rawBuffer, unsigned int rawSize, std::shared_ptr<AssetHandle> handle)
	{
		SDL_RWops *rw = SDL_RWFromMem(rawBuffer, rawSize);
		SDL_Surface *surface = IMG_Load_RW(rw, 1);

		if (surface == nullptr)
		{
			return false;
		}

		std::shared_ptr<ImageAssetPayloadData> payload = std::shared_ptr<ImageAssetPayloadData>(new ImageAssetPayloadData());
		payload->m_Surface = surface;
		handle->SetPayload(payload);
		return true;
	}

}}