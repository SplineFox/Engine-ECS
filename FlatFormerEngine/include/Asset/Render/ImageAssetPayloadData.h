#pragma once

#include "Asset/Asset.h"

#include <SDL.h>

namespace FlatFormer { namespace Asset {

	class ImageAssetPayloadData : public IAssetPayloadData
	{
		friend class PNGAssetLoader;
		friend class JPGAssetLoader;

	protected:
		SDL_Surface* m_Surface;

	public:
		ImageAssetPayloadData()
		{}

		~ImageAssetPayloadData()
		{
			SDL_FreeSurface(m_Surface);
		}

		std::string ToString() override { return "ImageAssetPayloadData"; };
		SDL_Surface* GetSurface() const { return m_Surface; }
	};

}}