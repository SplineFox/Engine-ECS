#include "Asset/Render/TTFAssetLoader.h"

#include "Asset/AssetHandle.h"

namespace FlatFormer { namespace Asset {

	std::string TTFAssetLoader::GetPattern()
	{
		return "*.ttf";
	}


	bool TTFAssetLoader::IsUseRawFile()
	{
		return false;
	}


	bool TTFAssetLoader::IsDiscardRawBufferAfterLoad()
	{
		return true;
	}


	unsigned int TTFAssetLoader::GetLoadedResourceSize(char* rawBuffer, unsigned int rawSize)
	{
		return rawSize;
	}


	bool TTFAssetLoader::LoadResource(char* rawBuffer, unsigned int rawSize, std::shared_ptr<AssetHandle> handle)
	{
		SDL_RWops *rw = SDL_RWFromMem(rawBuffer, rawSize);

		int fontSize = 16;
		TTF_Font* font = TTF_OpenFontRW(rw, 1, fontSize);

		if (font == nullptr)
		{
			return false;
		}

		std::shared_ptr<FontAssetPayloadData> payload = std::shared_ptr<FontAssetPayloadData>(new FontAssetPayloadData());
		payload->m_Font = font;
		handle->SetPayload(payload);
		return true;
	}

}}