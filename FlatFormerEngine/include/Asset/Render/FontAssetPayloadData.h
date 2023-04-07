#pragma once

#include "Asset/Asset.h"

#include <SDL.h>
#include <SDL_ttf.h>

namespace FlatFormer {
	namespace Asset {

		class FontAssetPayloadData : public IAssetPayloadData
		{
			friend class TTFAssetLoader;

		protected:
			TTF_Font* m_Font;

		public:
			FontAssetPayloadData()
			{}

			~FontAssetPayloadData()
			{
				TTF_CloseFont(m_Font);
			}

			std::string ToString() override { return "ImageAssetPayloadData"; };
			TTF_Font* GetFont() const { return m_Font; }
		};

	}
}